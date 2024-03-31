// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Audio/Implementations/WASAPI.hh>

#include <functiondiscoverykeys_devpkey.h>

namespace Cell::Audio::Implementations::WASAPI {

Wrapped<Collection::List<DeviceInfo>, Result> Subsystem::DiscoverAvailableRenderers() {
    IMMDeviceCollection* collection = nullptr;
    HRESULT result = this->enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &collection);
    switch (result) {
    case S_OK: {
        break;
    }

    default: {
        System::Panic("IMMDeviceEnumerator::EnumAudioEndpoints failed");
    }
    }

    uint32_t count = 0;
    result = collection->GetCount(&count);
    switch (result) {
    case S_OK: {
        break;
    }

    default: {
        System::Panic("IMMDeviceCollection::GetCount failed");
    }
    }

    if (count == 0) {
        return Result::NothingFound;
    }

    Collection::List<DeviceInfo> infos(count);

    for (uint32_t i = 0; i < count; i++) {
        IMMDevice* device = nullptr;
        result = collection->Item(i, &device);
        switch (result) {
        case S_OK: {
            break;
        }

        default: {
            System::Panic("IMMDeviceCollection::GetCount failed");
        }
        }

        LPWSTR id = nullptr;
        result = device->GetId(&id);
        switch (result) {
        case S_OK: {
            break;
        }

        default: {
            System::Panic("IMMDevice::GetId failed");
        }
        }

        IPropertyStore* properties = nullptr;
        result = device->OpenPropertyStore(STGM_READ, &properties);
        switch (result) {
        case S_OK: {
            break;
        }

        default: {
            System::Panic("IMMDevice::OpenPropertyStore failed");
        }
        }

        PROPVARIANT property = { };
        result = properties->GetValue(PKEY_Device_FriendlyName, &property);
        switch (result) {
        case S_OK: {
            break;
        }

        default: {
            System::Panic("IPropertyStore::GetValue failed");
        }
        }

        const size_t idSize = wcslen(id);
        infos[i].identifier = Memory::Allocate<wchar_t>(idSize + 1);
        Memory::Copy<wchar_t>((wchar_t*)infos[i].identifier, id, idSize);

        infos[i].name = String::FromPlatformWideString(property.pwszVal).Unwrap();

        properties->Release();
        CoTaskMemFree(id);
        device->Release();
    }

    collection->Release();
    return infos;
}

}
