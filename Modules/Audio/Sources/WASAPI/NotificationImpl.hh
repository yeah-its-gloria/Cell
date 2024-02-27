// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/Implementations/WASAPI.hh>

namespace Cell::Audio::Implementations::WASAPI {

class CellMMNotificationClient final : public IMMNotificationClient {
public:
    CELL_FUNCTION CellMMNotificationClient(IMMDeviceEnumerator* enumerator);
    CELL_FUNCTION ~CellMMNotificationClient();

// IUnknown

    CELL_FUNCTION ULONG AddRef();
    CELL_FUNCTION ULONG Release();
    CELL_FUNCTION HRESULT QueryInterface(const IID& id, void** ppvInterface);

// IMMNotificationClient

    CELL_FUNCTION HRESULT OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR id);
    CELL_FUNCTION HRESULT OnDeviceAdded(LPCWSTR id);
    CELL_FUNCTION HRESULT OnDeviceRemoved(LPCWSTR id);
    CELL_FUNCTION HRESULT OnDeviceStateChanged(LPCWSTR id, DWORD state);
    CELL_FUNCTION HRESULT OnPropertyValueChanged(LPCWSTR id, const PROPERTYKEY key);

private:
    LONG refCount;
    IMMDeviceEnumerator* enumerator;
};

}
