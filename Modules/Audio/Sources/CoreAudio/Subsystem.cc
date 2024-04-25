// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Audio/Implementations/CoreAudio.hh>
#include <Cell/System/Panic.hh>

#include <AudioToolbox/AudioFormat.h>

Cell::Audio::DeviceInfo::~DeviceInfo() {
    // ...
}

namespace Cell::Audio::Implementations::CoreAudio {

static const AudioObjectPropertyAddress DeviceListInfo = {
    kAudioHardwarePropertyDevices,
    kAudioObjectPropertyScopeGlobal,
    kAudioObjectPropertyElementMain
};

static const AudioObjectPropertyAddress DeviceNameInfo = {
    kAudioObjectPropertyName,
    kAudioObjectPropertyScopeGlobal,
    kAudioObjectPropertyElementMain
};

static const AudioObjectPropertyAddress DeviceCaptureConfigurationInfo = {
    kAudioDevicePropertyStreamConfiguration,
    kAudioDevicePropertyScopeInput,
    kAudioObjectPropertyElementMain
};

static const AudioObjectPropertyAddress DeviceRenderConfigurationInfo = {
    kAudioDevicePropertyStreamConfiguration,
    kAudioDevicePropertyScopeOutput,
    kAudioObjectPropertyElementMain
};

static const AudioObjectPropertyAddress DeviceUIDCaptureInfo = {
    kAudioDevicePropertyDeviceUID,
    kAudioDevicePropertyScopeInput,
    kAudioObjectPropertyElementMain
};

static const AudioObjectPropertyAddress DeviceUIDRenderInfo = {
    kAudioDevicePropertyDeviceUID,
    kAudioDevicePropertyScopeOutput,
    kAudioObjectPropertyElementMain
};

static const AudioObjectPropertyAddress DeviceDefaultRendererInfo = {
    kAudioHardwarePropertyDefaultOutputDevice,
    kAudioObjectPropertyScopeGlobal,
    kAudioObjectPropertyElementMain
};

CELL_FUNCTION_INTERNAL AudioStreamBasicDescription MakeStreamDescription(const Format& format) {
    uint32_t bits = 0;
    bool isFloat  = false;

    switch (format.type) {
    case FormatType::Float32PCM: {
        bits    = 32;
        isFloat = true;
        break;
    }

    case FormatType::S16PCM: {
        bits    = 16;
        isFloat = false;
        break;
    }

    default: {
        CELL_UNIMPLEMENTED
    }
    }

    return {
        .mSampleRate       = (float)format.rate,
        .mFormatID         = kAudioFormatLinearPCM,
        .mFormatFlags      = (isFloat ? kAudioFormatFlagIsFloat : kAudioFormatFlagIsSignedInteger) | kAudioFormatFlagIsPacked,
        .mBytesPerPacket   = (UInt32)(format.channels * (bits / 8)),
        .mFramesPerPacket  = 1,
        .mBytesPerFrame    = (UInt32)(format.channels * (bits / 8)),
        .mChannelsPerFrame = format.channels,
        .mBitsPerChannel   = bits,
        .mReserved         = 0
    };
}

Wrapped<Subsystem*, Result> Subsystem::New(const String& title) {
    (void)(title);

    return new Subsystem();
}

Subsystem::~Subsystem() {
    // ...
}

Wrapped<IRenderer*, Result> Subsystem::CreateRenderer(const Format& format) {
    (void)(format);

    AudioDeviceID deviceID;
    UInt32 deviceIDSize = sizeof(AudioDeviceID);

    OSStatus result = AudioObjectGetPropertyData(kAudioObjectSystemObject, &DeviceDefaultRendererInfo, 0, nullptr, &deviceIDSize, &deviceID);
    CELL_ASSERT(result == kAudioHardwareNoError);

    DeviceInfo stub = {
        .identifier = (void*)(uintptr_t)deviceID,
        .name       = ""
    };

    return this->CreateRenderer(stub, format);
}

Wrapped<IRenderer*, Result> Subsystem::CreateRenderer(const DeviceInfo& info, const Format& format) {
    AudioDeviceID id = (AudioDeviceID)(uintptr_t)info.identifier;

    Renderer* renderer = new Renderer(this, id);

    const AudioStreamBasicDescription streamDescription = MakeStreamDescription(format);
    OSStatus result = AudioQueueNewOutput(&streamDescription, Renderer::OutputBufferReadyCallback, renderer, CFRunLoopGetMain(), kCFRunLoopDefaultMode, 0, &renderer->queue);
    switch (result) {
    case kAudioHardwareNoError: {
        break;
    }

    case kAudioFormatUnsupportedDataFormatError: {
        return Result::UnsupportedFormat;
    }

    default: {
        System::Panic("AudioQueueNewOutput failed");
    }
    }

    (void)(DeviceUIDRenderInfo);

    // TODO: setting the device ID currently causes crashes inside AudioHardware
    /*CFStringRef deviceUID;
    UInt32 deviceUIDSize = sizeof(CFStringRef);
    result = AudioObjectGetPropertyData(id, &DeviceUIDRenderInfo, 0, NULL, &deviceUIDSize, &deviceUID);
    CELL_ASSERT(result == kAudioHardwareNoError);

    result = AudioQueueSetProperty(renderer->queue, kAudioQueueProperty_CurrentDevice, &deviceUID, deviceUIDSize);
    CELL_ASSERT(result == kAudioHardwareNoError);

    CFRelease(deviceUID);*/

    AudioQueueBufferRef buffer = nullptr;
    result = AudioQueueAllocateBuffer(renderer->queue, streamDescription.mBytesPerPacket * streamDescription.mSampleRate * 2, &buffer);
    CELL_ASSERT(result == kAudioHardwareNoError);

    Memory::Clear((uint8_t*)buffer->mAudioData, buffer->mAudioDataBytesCapacity);
    buffer->mAudioDataByteSize = buffer->mAudioDataBytesCapacity;

    result = AudioQueueEnqueueBuffer(renderer->queue, buffer, 0, nullptr);
    CELL_ASSERT(result == kAudioHardwareNoError);

    return renderer;
}

Wrapped<ICapturer*, Result> Subsystem::CreateLoopback(const DeviceInfo& info, const Format& format) {
    (void)(info); (void)(format);

    (void)(DeviceUIDCaptureInfo);
    (void)(DeviceCaptureConfigurationInfo);

    CELL_UNIMPLEMENTED
}

Wrapped<Collection::List<DeviceInfo>, Result> Subsystem::DiscoverAvailableRenderers() {
    UInt32 count = 0;
    OSStatus result = AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &DeviceListInfo, 0, nullptr, &count);
    CELL_ASSERT(result == kAudioHardwareNoError);

    // AudioObjectGetPropertyDataSize counts bytes, not objects
    ScopedBlock<AudioDeviceID> deviceIDs = Memory::Allocate<AudioDeviceID>(count / sizeof(AudioObjectID));

    result = AudioObjectGetPropertyData(kAudioObjectSystemObject, &DeviceListInfo, 0, nullptr, &count, deviceIDs);
    CELL_ASSERT(result == kAudioHardwareNoError);

    Collection::List<DeviceInfo> devices;

    for (size_t i = 0; i < count / 4; i++) {
        UInt32 bufferCount = 0;
        result = AudioObjectGetPropertyDataSize(deviceIDs[i], &DeviceRenderConfigurationInfo, 0, nullptr, &bufferCount);
        CELL_ASSERT(result == kAudioHardwareNoError);

        if (bufferCount == 0) {
            continue;
        }

        CFStringRef nameString = nullptr;
        UInt32 nameSize = sizeof(CFStringRef);
        result = AudioObjectGetPropertyData(deviceIDs[i], &DeviceNameInfo, 0, nullptr, &nameSize, &nameString);
        CELL_ASSERT(result == kAudioHardwareNoError);

        devices.Append(DeviceInfo {
            .identifier = (void*)(uintptr_t)deviceIDs[i],
            .name       = String(CFStringGetCStringPtr(nameString, kCFStringEncodingUTF8))
        });

        CFRelease(nameString);
    }

    if (devices.GetCount() == 0) {
        return Result::NothingFound;
    }

    return devices;
}

}
