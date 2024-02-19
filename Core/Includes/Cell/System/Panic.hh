// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::System {

// Panics with the given message.
CELL_NORETURN CELL_FUNCTION __attribute__((format(printf, 1, 2))) void Panic(const char* CELL_NONNULL message, ...);

// Attempts to show the user a message box saying an error occurred, and then aborts.
// Mostly intended for release builds.
CELL_NORETURN CELL_FUNCTION void SoftPanic();

}

#ifdef CELL_CORE_SKIP_ASSERT
#define CELL_ASSERT(x) (void)(x)
#elifdef CELL_CORE_USE_EXTERNAL_ASSERT
#include <assert.h>
#define CELL_ASSERT(condition) assert(condition)
#elifdef CELL_CORE_USE_PANIC_ASSERT
#define CELL_ASSERT(condition) if (!(condition)) { Cell::System::Panic("Assert \"%s\" failed inside function %s at %s:%d", #condition, __FUNCTION__, __FILE__, __LINE__); } (void)(condition)
#else
#define CELL_ASSERT(condition) if (!(condition)) { Cell::System::SoftPanic(); } (void)(condition)
#endif

#define CELL_UNIMPLEMENTED { Cell::System::Panic("Call to unimplemented function \"%s\" at %s:%d", __FUNCTION__, __FILE__, __LINE__); }

#ifdef CELL_PLATFORM_WINDOWS
#define CELL_DEBUGBREAK __debugbreak()
#else
#define CELL_DEBUGBREAK __builtin_trap()
#endif

#define CELL_UNREACHABLE __builtin_unreachable()
#define CELL_STATIC_ASSERT static_assert
