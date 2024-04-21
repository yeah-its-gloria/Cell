// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Audio/Implementations/CoreAudio.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>

Cell::Audio::DeviceInfo::~DeviceInfo() {
    CELL_UNIMPLEMENTED
}

namespace Cell::Audio::Implementations::CoreAudio {

static const AudioObjectPropertyAddress DeviceListInfo = {
    kAudioHardwarePropertyDevices,
    kAudioObjectPropertyScopeGlobal,
    kAudioObjectPropertyElementMain
};

/*static const AudioObjectPropertyAddress DeviceInfoInfo = {
    kAudioObjectPropertyName,
    kAudioDevicePropertyScopeOutput,
    kAudioObjectPropertyElementMain
};*/

AudioStreamBasicDescription MakeFormat(const uint32_t sampleRate, const uint8_t bits, const uint8_t channels, const bool isFloat) {
    return {
        .mSampleRate       = (float)sampleRate,
        .mFormatID         = kAudioFormatLinearPCM,
        .mFormatFlags      = isFloat ? kAudioFormatFlagIsFloat : kAudioFormatFlagIsSignedInteger,
        .mBytesPerPacket   = (UInt32)(channels * (bits / 2)),
        .mFramesPerPacket  = 1,
        .mBytesPerFrame    = (UInt32)(channels * (bits / 2)),
        .mChannelsPerFrame = channels,
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
    UInt32 count = 0;
    OSStatus result = AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &DeviceListInfo, 0, nullptr, &count);
    CELL_ASSERT(result == kAudioHardwareNoError);

    // AudioObjectGetPropertyDataSize counts bytes, not objects
    ScopedBlock<AudioDeviceID> devices = Memory::Allocate<AudioDeviceID>(count / 4);

    result = AudioObjectGetPropertyData(kAudioObjectSystemObject, &DeviceListInfo, 0, nullptr, &count, devices);
    CELL_ASSERT(result == kAudioHardwareNoError);

    count /= sizeof(AudioObjectID);

    /*CFStringRef nameString = nullptr;
    UInt32 nameSize = sizeof(CFStringRef);
    result = AudioObjectGetPropertyData(devices[1], &DeviceInfoInfo, 0, nullptr, &nameSize, &nameString);
    CELL_ASSERT(result == kAudioHardwareNoError);

    const char* nameStr = CFStringGetCStringPtr(nameString, kCFStringEncodingUTF8);
    System::Log("Initializing \"%\"", nameStr);

    CFRelease(nameString);*/

    Renderer* renderer = new Renderer(this, devices[1]);

    const auto aFormat = [&]() {
        switch (format.type) {
        case FormatType::Float32PCM: {
            return MakeFormat(format.rate, 32, format.channels, true);
        }

        case FormatType::S16PCM: {
            return MakeFormat(format.rate, 16, format.channels, false);
        }

        default: {
            CELL_UNIMPLEMENTED
        }
        }
    }();

    result = AudioQueueNewOutput(&aFormat, Renderer::OutputBufferFunc, renderer, CFRunLoopGetMain(), kCFRunLoopDefaultMode, 0, &renderer->queue);
    CELL_ASSERT(result == kAudioHardwareNoError);

    return renderer;
}

Wrapped<IRenderer*, Result> Subsystem::CreateRenderer(const DeviceInfo& info, const Format& format) {
    (void)(info); (void)(format);

    CELL_UNIMPLEMENTED
}

Wrapped<ICapturer*, Result> Subsystem::CreateLoopback(const DeviceInfo& info, const Format& format) {
    (void)(info); (void)(format);

    CELL_UNIMPLEMENTED
}

Wrapped<Collection::List<DeviceInfo>, Result> Subsystem::DiscoverAvailableRenderers() {
    CELL_UNIMPLEMENTED
}

}
