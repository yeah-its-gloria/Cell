
// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include  "NotificationImpl.hh"

namespace Cell::Audio::Implementations::WASAPI {

CellMMNotificationClient::CellMMNotificationClient(IMMDeviceEnumerator* enumerator) : refCount(1), enumerator(enumerator) {
    const HRESULT result = enumerator->RegisterEndpointNotificationCallback(this);
    switch (result) {
    case S_OK: {
        break;
    }

    default: {
        System::Panic("IMMDeviceEnumerator::RegisterEndpointNotificationCallback failed");
    }
    }
}

CellMMNotificationClient::~CellMMNotificationClient() {
    this->enumerator->Release();
}

ULONG CellMMNotificationClient::AddRef() {
    return InterlockedIncrement(&refCount);
}

ULONG CellMMNotificationClient::Release() {
    const ULONG newCount = InterlockedDecrement(&refCount);
    if (newCount == 0) {
        delete this;
    }

    return newCount;
}

HRESULT CellMMNotificationClient::QueryInterface(const IID& id, void** ppvInterface) {
    if (id == IID_IUnknown) {
        *ppvInterface = (IUnknown*)this;
    } else if (id == __uuidof(IMMNotificationClient)) {
        *ppvInterface = (IMMNotificationClient*)this;
    } else {
        *ppvInterface = nullptr;
        return E_NOINTERFACE;
    }

    this->AddRef();
    return S_OK;
}

HRESULT CellMMNotificationClient::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR id) {
    (void)(flow);
    (void)(role);
    (void)(id);

    // TODO: inform any active renderer that we need to switch clients

    return S_OK;
}

HRESULT CellMMNotificationClient::OnDeviceAdded(LPCWSTR id) {
    (void)(id);

    return S_OK;
};

HRESULT CellMMNotificationClient::OnDeviceRemoved(LPCWSTR id) {
    (void)(id);

    return S_OK;
}

HRESULT CellMMNotificationClient::OnDeviceStateChanged(LPCWSTR id, DWORD state) {
    (void)(id);
    (void)(state);

    return S_OK;
}

HRESULT CellMMNotificationClient::OnPropertyValueChanged(LPCWSTR id, const PROPERTYKEY key) {
    (void)(id);
    (void)(key);

    return S_OK;
}

}
