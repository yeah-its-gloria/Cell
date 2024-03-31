// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Utilities {

template <typename T> concept CompleteType = requires { sizeof(T); };

template <typename T> constexpr bool IsClassType = __is_class(T);
template <typename T> concept ClassType = requires { IsClassType<T>; };

template <typename> constexpr bool IsPointerType = false;
template <CompleteType T> constexpr bool IsPointerType<T*> = true;

template <typename T> constexpr bool ImplementsCellObject = __is_base_of(Object, T);
template <CompleteType T> constexpr bool ImplementsCellObject<T*> = __is_base_of(Object, T);

template <ClassType T> constexpr bool IsDeletable = IsPointerType<T> && ImplementsCellObject<T>;

}
