// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/HID.hh>
#include <Cell/Memory/Allocator.hh>
#include <Cell/System/Event.hh>
#include <Cell/System/Mutex.hh>
#include <Cell/System/Panic.hh>

#include <IOKit/hid/IOHIDManager.h>

namespace Cell::IO::HID {

// TODO: restructure stuff to fit this into Cell properly
//       handle device disconnects better (at the moment it just appears to fail to respond)

struct devData {
    IOHIDDeviceRef device;
    uint8_t reportJunk[64];
    System::Mutex* mutex;
    System::Event* event;
};

CELL_FUNCTION_INTERNAL void reportCallback(void* context, IOReturn result, void* sender, IOHIDReportType type, uint32_t id, uint8_t* data, CFIndex length) {
    (void)(sender);
    (void)(type);
    (void)(id);

    CELL_ASSERT(context != nullptr && result == kIOReturnSuccess && length == 64);

    devData* dev = (devData*)context;

    dev->mutex->Lock();

    Memory::Copy(dev->reportJunk, data, 64);
    dev->event->Signal();

    dev->mutex->Unlock();
}

CELL_FUNCTION_INTERNAL void updateLoop() {
    while (true) {
		const CFRunLoopRunResult runResult = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1 * MSEC_PER_SEC, FALSE);
        if (runResult == kCFRunLoopRunFinished || runResult == kCFRunLoopRunTimedOut) {
            break;
        }
	}
}

Wrapped<Device*, Result> Device::Open(const uint16_t vendorId, const uint16_t productId) {
    (void)(vendorId);
    (void)(productId);

    IOHIDManagerRef manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDManagerOptionIndependentDevices);
    CELL_ASSERT(manager != nullptr);

    IOReturn result = IOHIDManagerOpen(manager, kIOHIDManagerOptionIndependentDevices);
    CELL_ASSERT(result == kIOReturnSuccess);

    CFMutableDictionaryRef infoDict = CFDictionaryCreateMutable(kCFAllocatorDefault, kIOHIDOptionsTypeNone, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

    CFNumberRef vendor  = CFNumberCreate(kCFAllocatorDefault, kCFNumberShortType, &vendorId);
    CFNumberRef product = CFNumberCreate(kCFAllocatorDefault, kCFNumberShortType, &productId);

    CFDictionarySetValue(infoDict, CFSTR(kIOHIDVendorIDKey),  vendor);
    CFDictionarySetValue(infoDict, CFSTR(kIOHIDProductIDKey), product);

	IOHIDManagerSetDeviceMatching(manager, infoDict);
    IOHIDManagerSetDispatchQueue(manager, dispatch_get_main_queue());

    IOHIDManagerActivate(manager);

    updateLoop();

    CFRelease(vendor);
    CFRelease(product);
    CFRelease(infoDict);

	CFSetRef deviceSet = IOHIDManagerCopyDevices(manager);
    if (deviceSet == nullptr) {
        return Result::NotFound;
    }

	IOHIDDeviceRef* devices = Memory::Allocate<IOHIDDeviceRef>(CFSetGetCount(deviceSet));
    CFSetGetValues(deviceSet, (const void**)devices);

    CFRelease(deviceSet);

    IOHIDDeviceRef device = devices[0];
    Memory::Free(devices);

    result = IOHIDDeviceOpen(device, 0);
    CELL_ASSERT(result == kIOReturnSuccess);

    devData* data = Memory::Allocate<devData>();

    data->device = device;
    data->event = new System::Event();
    data->mutex = new System::Mutex();

    IOHIDDeviceSetDispatchQueue(device, dispatch_get_main_queue());
    IOHIDDeviceRegisterInputReportCallback(device, data->reportJunk, 64, reportCallback, data);

    IOHIDDeviceActivate(device);

    updateLoop();

    result = IOHIDManagerClose(manager, 0);
    CELL_ASSERT(result == kIOReturnSuccess);

    return new Device((uintptr_t)data, ConnectionType::USB);
}

Device::~Device() {
    devData* data = (devData*)this->impl;

    IOHIDDeviceCancel(data->device);
    updateLoop();

    IOHIDDeviceClose(data->device, 0);

    delete data->event;
    delete data->event;

    Memory::Free(data);
}

Result Device::Read(Memory::IBlock& data, const uint32_t milliseconds) {
    if (data.GetSize() != 64) {
        return Result::InvalidParameters;
    }

    devData* dev = (devData*)this->impl;

    System::Result result = dev->event->Wait(milliseconds);
    switch (result) {
    case System::Result::Success: {
        dev->mutex->Lock();

        Memory::Copy(data.AsBytes(), dev->reportJunk, 64);
        dev->event->Reset();

        dev->mutex->Unlock();
        break;
    }

    case System::Result::Timeout: {
        return Result::Timeout;
    }

    default: {
        CELL_UNREACHABLE;
    }
    }

    return Result::Success;
}

Result Device::Write(const Memory::IBlock& data, const uint32_t milliseconds) {
    if (data.GetSize() == 0 || milliseconds == 0) {
        return Result::InvalidParameters;
    }

    devData* dev = (devData*)this->impl;

    const IOReturn result = IOHIDDeviceSetReportWithCallback(dev->device,
                                                             kIOHIDReportTypeOutput,
                                                             data.AsBytes()[0],
                                                             data.AsBytes(),
                                                             data.GetSize(),
                                                             milliseconds,
                                                             nullptr,
                                                             nullptr);
    CELL_ASSERT(result == kIOReturnSuccess);

    return Result::Success;
}

Wrapped<Capabilities, Result> Device::GetCapabilities() {
    CELL_UNIMPLEMENTED
}

}
