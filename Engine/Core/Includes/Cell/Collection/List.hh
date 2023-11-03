// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/Enumerable.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Collection {

// Represents a generic list.
template <typename T> class List : public IEnumerable<T> {
public:
    // Creates a list of empty T elements with the given expected number of elements.
    CELL_INLINE explicit List(const size_t count = 0) : data(nullptr), count(count) {
        if (this->count > 0) {
            this->data = System::AllocateMemory<T>(this->count);
        }
    }

    // Creates a one T sized list with an initial element.
    CELL_INLINE explicit List(const T& initial) : count(1) {
        this->data = System::AllocateMemory<T>();

        System::CopyMemory<T>(&this->data[0], &initial);
    }

    // Creates a count T sized list with all elements being the same given initial one.
    CELL_INLINE explicit List(const T& initial, const size_t count) : count(count) {
        CELL_ASSERT(count > 1);

        this->data = System::AllocateMemory<T>(this->count);
        for (size_t i = 0; i < this->count; i++) {
            System::CopyMemory<T>(&this->data[i], &initial);
        }
    }

    // Copies another list.
    CELL_INLINE List(const List<T>& list) : data(nullptr), count(0) {
        this->count = list.count;
        if (this->count == 0) {
            return;
        }

        this->data = System::AllocateMemory<T>(this->count);
        System::CopyMemory<T>(this->data, list.data, this->count);
    }

    // Destructs this list and frees its contents.
    CELL_INLINE ~List() {
        if (this->data != nullptr) {
            System::FreeMemory(this->data);
        }
    }

    // Appends an element to the end of the list.
    CELL_INLINE size_t Append(const T data) {
        if (this->count == 0) {
            this->data = System::AllocateMemory<T>();
            this->count++;
        } else {
            System::ReallocateMemory<T>(&this->data, ++this->count);
        }

        System::CopyMemory<T>(&this->data[this->count - 1], &data);
        return this->count - 1;
    }

    // Removes the element at the given index.
    CELL_INLINE void Remove(const size_t index) {
        CELL_ASSERT(index < this->count);

        if (index < this->count - 1) {
            System::CopyMemory<T>(this->data + index, this->data + index + 1, this->count - index);
        }

        System::ReallocateMemory<T>(&this->data, --this->count);
    }

    // Removes the given element at the first found index which equals it.
    CELL_INLINE bool Remove(const T data) {
        CELL_ASSERT(this->count > 0);

        for (size_t i = 0; i < this->count; i++) {
            if (this->data[i] == data) {
                Remove(i);
                return true;
            }
        }

        return false;
    }

    // Resets the entire list to be empty.
    CELL_INLINE void Reset() {
        System::FreeMemory(this->data);

        this->count = 0;
        this->data = nullptr;
    }

    // Returns the count of stored (empty or otherwise) elements.
    CELL_NODISCARD CELL_INLINE size_t GetCount() const {
        return this->count;
    }

    // Sets the new count of elements.
    // If this is smaller, elements at the end of the list are discarded.
    // A bigger count will increase the list at its end.
    CELL_INLINE void SetCount(const size_t count) {
        CELL_ASSERT(count > 0);

        if (this->count == 0) {
            this->data = System::AllocateMemory<T>(count);
        } else {
            System::ReallocateMemory<T>(&this->data, count);
        }

        this->count = count;
    }

    // Retrieves a pointer to a given element.
    CELL_NODISCARD CELL_INLINE T* GetPointer(const size_t index) {
        CELL_ASSERT(index < this->count);

        return __builtin_addressof(this->data[index]);
    }

    // Retrieves the element at the specific index.
    CELL_NODISCARD CELL_INLINE T& operator [](const size_t index) {
        CELL_ASSERT(index < this->count);

        return this->data[index];
    }

    // Returns the raw data block stored in the list.
    CELL_NODISCARD CELL_INLINE T* operator &() {
        return this->data;
    }

    // Copies another list into this currently alive list.
    // This overwrites the contents.
    CELL_NODISCARD CELL_INLINE List<T>& operator =(const List<T>& list) {
        if (this == list) {
            return *this;
        }

        if (this->count > 0) {
            System::FreeMemory(this->data);
            this->data = nullptr;
        }

        this->count = list.count;
        if (this->count > 0) {
            this->data = System::AllocateMemory<T>(this->count);
            System::CopyMemory<T>(this->data, list.data, this->count);
        }

        return *this;
    }

    // begin operator for loops.
    CELL_NODISCARD CELL_INLINE T* begin() {
        return &this->data[0];
    }

    // end operator for loops.
    CELL_NODISCARD CELL_INLINE T* end() {
        return &this->data[this->count];
    }

private:
    T* data;
    size_t count;
};

}
