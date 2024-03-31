// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/Enumerable.hh>
#include <Cell/Memory/Allocator.hh>
#include <Cell/Memory/UnownedBlock.hh>
#include <Cell/Utilities/Concepts.hh>

// Yes, I allow this
#include <initializer_list>

namespace Cell::Collection {

template <typename T> class List;

template <typename T> class ListView : public IEnumerable<const T> {
friend List<T>;

public:
    CELL_FUNCTION_TEMPLATE ~ListView() = default;

    // Retrieves the count of elements in this enumerable.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE size_t GetCount() const override {
        return this->size;
    }

    // Retrieves the element at the given index.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T& operator [] (const size_t index) override {
        return this->block[index];
    }

    // Retrieves the element at the given index.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE const T& operator [] (const size_t index) const override {
        return this->block[index];
    }

    // Begin operator for foreach operations.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T* begin() override {
        return this->block;
    }

    // End operator for foreach operations.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T* end() override {
        return this->block + this->size;
    }

    // Begin operator for constant foreach operations.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE const T* begin() const override {
        return this->block;
    }

    // End operator for constant foreach operations.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE const T* end() const override {
        return this->block + this->size;
    }

private:
    ListView(T* b, const size_t s) : block(b), size(s) { }

    T* block;
    const size_t size;
};

// Generic list implementation.
template <typename T> class List : public IEnumerable<T> {
public:
    // Creates a list of empty T elements with the given expected number of elements.
    CELL_FUNCTION_TEMPLATE explicit List(const size_t count = 0) : data(nullptr), count(count) {
        if (this->count > 0) {
            this->data = Memory::Allocate<T>(this->count);
        }
    }

    // Creates a list with an initial element.
    CELL_FUNCTION_TEMPLATE explicit List(const T& initial) : count(1) {
        this->data = Memory::Allocate<T>();

        Memory::Copy<T>(&this->data[0], &initial);
    }

    // Creates a list with the given initial element setting every element in the list, with the list containing count entries.
    CELL_FUNCTION_TEMPLATE explicit List(const T& initial, const size_t count) : count(count) {
        CELL_ASSERT(count > 1);

        this->data = Memory::Allocate<T>(this->count);
        for (size_t i = 0; i < this->count; i++) {
            Memory::Copy<T>(&this->data[i], &initial);
        }
    }

    // Creates a list with all given elements.
    CELL_FUNCTION_TEMPLATE List(const std::initializer_list<T> list) : count(list.size()) {
        this->data = Memory::Allocate<T>(this->count);
        Memory::Copy<T>(this->data, list.begin(), this->count);
    }

    // Creates a list by copying the entries in the given list.
    CELL_FUNCTION_TEMPLATE List(const List<T>& list) : data(nullptr), count(0) {
        if (list.count == 0) {
            return;
        }

        this->count = list.count;
        this->data = Memory::Allocate<T>(this->count);
        Memory::Copy<T>(this->data, list.data, this->count);
    }

    // Destructs this list by deleting every object stored and freeing its memory.
    CELL_FUNCTION_TEMPLATE ~List() requires Utilities::IsDeletable<T> {
        if (this->data == nullptr) {
            return;
        }

        for (size_t i = 0; i < this->count; i++) {
            delete this->data[i];
        }

        Memory::Free(this->data);
    }

    // Destructs this list's memory.
    CELL_FUNCTION_TEMPLATE ~List() {
        if (this->data == nullptr) {
            return;
        }

        Memory::Free(this->data);
    }

    // Appends an element to the end of the list.
    //
    // Note that appending past the preallocated entry block will cause the list
    //  to automatically reallocate a larger list.
    // this can move the list and its elements to a different memory location,
    //  invalidating pointers.
    CELL_FUNCTION_TEMPLATE void Append(const T data) {
        if (this->count == 0) {
            this->data = Memory::Allocate<T>();
        } else {
            Memory::Reallocate<T>(this->data, this->count + 1);
        }

        this->data[this->count++] = data;
    }

    // Removes the list entry at the given index.
    CELL_FUNCTION_TEMPLATE void Remove(const size_t index) {
        CELL_ASSERT(index < this->count);

        if (index < this->count - 1) {
            Memory::Copy<T>(this->data + index, this->data + index + 1, this->count - index);
        }

        Memory::Reallocate<T>(this->data, --this->count);
    }

    // Removes the first entry matching the given data. Returns false if none was found.
    CELL_FUNCTION_TEMPLATE bool Remove(const T data) {
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
    CELL_FUNCTION_TEMPLATE void Reset() {
        if (this->data != nullptr) {
            Memory::Free(this->data);
        }

        this->count = 0;
        this->data = nullptr;
    }

    // Returns the number of stored entries.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE size_t GetCount() const override {
        return this->count;
    }

    // Changes the number of stored entries in this list.
    // If the count is smaller than the currently stored number of entries, it cuts away the remaining elements.
    // If the count is larger, empty entries are added to the end.
    CELL_FUNCTION_TEMPLATE void SetCount(const size_t count) {
        CELL_ASSERT(count > 0);

        if (this->count == 0) {
            this->data = Memory::Allocate<T>(count);
        } else {
            Memory::Reallocate<T>(this->data, count);
        }

        this->count = count;
    }

    // Retrieves a pointer to a given element.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T* GetPointer(const size_t index) {
        CELL_ASSERT(index < this->count);

        return this->data + index;
    }

    // Retrieves the entry at the specified index.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T& operator [] (const size_t index) override {
        CELL_ASSERT(index < this->count);

        return this->data[index];
    }

    // Retrieves the entry at the specified index.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE const T& operator [] (const size_t index) const override {
        CELL_ASSERT(index < this->count);

        return this->data[index];
    }

    // Returns this list as a block.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE Memory::UnownedBlock<T> AsBlock() {
        return Memory::UnownedBlock<T> { this->data, this->count };
    }

    // Returns the raw data pointer for the list. It's advised to use AsBlock() instead.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T* AsRaw() {
        return this->data;
    }


    // Overwrites the contents of this list with the given list.
    CELL_FUNCTION_TEMPLATE List<T>& operator = (const List<T>& list) {
        if (this->data == list.data) {
            return *this;
        }

        if (this->count > 0) {
            Memory::Free(this->data);
            this->data = nullptr;
        }

        this->count = list.count;
        if (this->count > 0) {
            this->data = Memory::Allocate<T>(this->count);
            Memory::Copy<T>(this->data, list.data, this->count);
        }

        return *this;
    }

    // begin operator for loops.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T* begin() override {
        return this->data;
    }

    // end operator for loops.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T* end() override {
        return this->data + this->count;
    }

    // Begin operator for constant foreach operations.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE const T* begin() const override {
        return this->data;
    }

    // End operator for constant foreach operations.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE const T* end() const override {
        return this->data + this->count;
    }

    CELL_NODISCARD CELL_FUNCTION_TEMPLATE ListView<T> MakeView(const size_t offset, const size_t size) {
        CELL_ASSERT(offset + size < this->count);

        return ListView<T>(this->data + offset, size);
    }

private:
    T* data;
    size_t count;
};

}

