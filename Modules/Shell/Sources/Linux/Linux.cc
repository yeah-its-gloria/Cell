// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Shell/Implementations/Linux.hh>
#include <Cell/System/Log.hh>

namespace Cell::Shell::Implementations {

Result Linux::RunDispatchImpl() {
    const int result = wl_display_dispatch_pending(this->display);
    CELL_ASSERT(result > -1);

    if (this->isDone) {
        return Result::RequestedQuit;
    }

    this->isActivated = this->keyboardActive || this->pointerActive;
    return Result::Success;
}

Result Linux::RequestQuit() {
    this->isDone = true;
    return Result::Success;
}

Wrapped<Extent, Result> Linux::GetDrawableExtent() {
    return this->extent;
}

Result Linux::SetDrawableExtent(const Extent extent) {
    if (extent.width > INT32_MAX || extent.height > INT32_MAX) {
        return Result::InvalidParameters;
    }

    xdg_surface_set_window_geometry(this->xdgSurface, 0, 0, extent.width, extent.height);
    wl_surface_commit(this->surface);

    this->extent = extent;
    return Result::Success;
}

Result Linux::SetNewTitle(const String& title) {
    if (title.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock titleStr = title.ToCharPointer();
    xdg_toplevel_set_title(this->xdgToplevel, &titleStr);

    return Result::Success;
}

Result Linux::IndicateStatus(const ShellStatus status) {
    if (this->cursorShapeDevice == nullptr || !this->pointerActive) {
        return Result::Success;
    }

    wp_cursor_shape_device_v1_shape shape;

    switch (status) {
    case ShellStatus::Default: {
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_DEFAULT;
        break;
    }

    case ShellStatus::Working: {
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_WAIT;
        break;
    }

    default: {
        CELL_UNREACHABLE;
    }
    }

    wp_cursor_shape_device_v1_set_shape(this->cursorShapeDevice, this->pointerSerial, shape);
    return Result::Success;
}

Result Linux::CaptureState(const bool captured) {
    if (this->pointerConstraints == nullptr || this->pointerLocked == captured || !this->pointerActive) {
        return Result::Success;
    }

    this->pointerLocked = captured;
    if (captured) {
        this->pointerLock = zwp_pointer_constraints_v1_lock_pointer(this->pointerConstraints, this->surface, this->pointer, nullptr, ZWP_POINTER_CONSTRAINTS_V1_LIFETIME_PERSISTENT);
        if (this->pointerLock == nullptr) {
            System::Panic("zwp_pointer_constraints_v1_lock_pointer failed");
        }

        return Result::Success;
    }

    zwp_locked_pointer_v1_destroy(this->pointerLock);
    this->pointerLock = nullptr;

    return Result::Success;
}

Result Linux::Log(const String& text) {
    System::Log(text);
    return Result::Success;
}

Result Linux::LogClear() {
    return Result::Success;
}

}
