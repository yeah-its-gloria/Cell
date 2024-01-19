// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/Instance.hh>
#include <Cell/System/Panic.hh>

namespace Cell::OpenXR {

Result Instance::GetFunctionPointer(const char* name, PFN_xrVoidFunction* function) {
    XrResult result = xrGetInstanceProcAddr(this->instance, name, function);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    case XR_ERROR_FUNCTION_UNSUPPORTED: {
        return Result::FeatureMissing;
    }

    case XR_ERROR_RUNTIME_FAILURE: {
        return Result::RuntimeFailure;
    }

    case XR_ERROR_INSTANCE_LOST: {
        return Result::RuntimeLost;
    }

    case XR_ERROR_OUT_OF_MEMORY: {
        return Result::OutOfMemory;
    }

    default: {
        System::Panic("xrGetInstanceProcAddr failed");
    }
    }

    return Result::Success;
}

}
