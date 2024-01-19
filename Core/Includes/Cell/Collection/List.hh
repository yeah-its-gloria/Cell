// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/Enumerable.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Memory.hh>
#include <Cell/Utilities/Preprocessor.hh>

namespace Cell::Collection {

// Generic list implementation.
template <typename T> class List : public IEnumerable<T> {
public:
    // Creates a list of empty T elements with the given expected number of elements.
    CELL_INLINE explicit List(const size_t count = 0) : data(nullptr), count(count) {
        if (this->count > 0) {
            this->data = System::AllocateMemory<T>(this->count);
        }
    }

    // Creates a list with an initial element.
    CELL_INLINE explicit List(const T& initial) : count(1) {
        this->data = System::AllocateMemory<T>();

        System::CopyMemory<T>(&this->data[0], &initial);
    }

    // Creates a list with the given initial element setting every element in the list, with the list containing count entries.
    CELL_INLINE explicit List(const T& initial, const size_t count) : count(count) {
        CELL_ASSERT(count > 1);

        this->data = System::AllocateMemory<T>(this->count);
        for (size_t i = 0; i < this->count; i++) {
            System::CopyMemory<T>(&this->data[i], &initial);
        }
    }

    // Creates a list by copying the entries in the given list.
    CELL_INLINE List(const List<T>& list) : data(nullptr), count(0) {
        if (list.count == 0) {
            return;
        }

        this->count = list.count;
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
    //
    // Note that appending past the preallocated entry block will cause the list
    //  to automatically reallocate a larger list.
    // this can move the list and its elements to a different memory location,
    //  invalidating pointers.
    CELL_INLINE void Append(const T data) {
        if (this->count == 0) {
            this->data = System::AllocateMemory<T>();
            this->count++;
        } else {
            System::ReallocateMemory<T>(&this->data, ++this->count);
        }

        System::CopyMemory<T>(&this->data[this->count - 1], &data);
    }

    // Removes the list entry at the given index.
    CELL_INLINE void Remove(const size_t index) {
        CELL_ASSERT(index < this->count);

        if (index < this->count - 1) {
            System::CopyMemory<T>(this->data + index, this->data + index + 1, this->count - index);
        }

        System::ReallocateMemory<T>(&this->data, --this->count);
    }

    // Removes the first entry matching the given data. Returns false if none was found.
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

    // Resets the entire list by emptying its storage.
    CELL_INLINE void Reset() {
        if (this->data != nullptr) {
            System::FreeMemory(this->data);
        }

        this->count = 0;
        this->data = nullptr;
    }

    // Returns the number of stored entries.
    CELL_NODISCARD CELL_INLINE size_t GetCount() const override {
        return this->count;
    }

    // Changes the number of stored entries in this list.
    // If the count is smaller than the currently stored number of entries, it cuts away the remaining elements.
    // If the count is larger, empty entries are added to the end.
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

        return this->data + index;
    }

    // Retrieves the entry at the specified index.
    CELL_NODISCARD CELL_INLINE T& operator [] (const size_t index) override {
        CELL_ASSERT(index < this->count);

        return this->data[index];
    }

    // Returns this list as a block.
    CELL_NODISCARD CELL_INLINE System::UnownedBlock<T> AsBlock() {
        return System::UnownedBlock<T> { this->data, this->count };
    }

    // Returns the raw data pointer for the list. It's advised to use AsBlock() instead.
    CELL_NODISCARD CELL_INLINE T* AsRaw() {
        return this->data;
    }

    // Overwrites the contents of this list with the given list.
    CELL_NODISCARD CELL_INLINE List<T>& operator = (const List<T>& list) {
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
    CELL_NODISCARD CELL_INLINE T* begin() override {
        return this->data;
    }

    // end operator for loops.
    CELL_NODISCARD CELL_INLINE T* end() override {
        return this->data + this->count;
    }

private:
    T* data;
    size_t count;
};

template<> CELL_INLINE List<Cell::Object*>::~List() {
    if (this->data != nullptr) {
        for (size_t i = 0; i < this->count; i++) {
            delete this->data[i];
        }

        System::FreeMemory(this->data);
    }
}

}

