// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/Collection/Enumerable.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Collection {

// Basic dictionary type.
template <typename K, typename V> class Dictionary : public IEnumerable<K> {
public:
    // Creates an empty dictionary.
    CELL_INLINE explicit Dictionary(const size_t size = 0) : count(size) {
        if (this->count > 0) {
            this->keys = System::AllocateMemory<K>(this->count);
            this->values = System::AllocateMemory<V>(this->count);
        }
    }

    // Creates a dictionary with an initial set.
    CELL_INLINE Dictionary(const K& key, const V& value) : count(1) {
        this->keys = System::AllocateMemory<K>();
        this->values = System::AllocateMemory<V>();

        System::CopyMemory<K>(&this->keys[0], &key);
        System::CopyMemory<V>(&this->values[0], &value);
    }

    // Copies a dictionary.
    CELL_INLINE Dictionary(const Dictionary<K, V>& dictionary) : count(0) {
        if (this->keys != nullptr) {
            System::FreeMemory(this->keys);
            System::FreeMemory(this->values);

            this->values = nullptr;
        }

        CELL_ASSERT(this->values == nullptr);

        this->count = dictionary.count;
        this->keys = System::AllocateMemory<K>(this->count);
        this->values = System::AllocateMemory<V>(this->count);

        System::CopyMemory<K>(this->keys, dictionary.keys, this->count);
        System::CopyMemory<V>(this->values, dictionary.values, this->count);
    }

    // Destructs this dictionary and frees its contents.
    CELL_INLINE ~Dictionary() {
        System::FreeMemory(this->values);
        System::FreeMemory(this->keys);
    }

    // Appends an entry to the end of the dictionary.
    CELL_INLINE void Append(const K& key, const V& value) {
        if (this->count == 0) {
            this->keys = System::AllocateMemory<K>();
            this->values = System::AllocateMemory<V>();
            this->count++;
        } else {
            System::ReallocateMemory<K>(&this->keys, ++this->count);
            System::ReallocateMemory<V>(&this->values, this->count);
        }

        System::CopyMemory<K>(&this->keys[this->count - 1], &key);
        System::CopyMemory<V>(&this->values[this->count - 1], &value);
    }

    // Removes the entry at the given index.
    CELL_INLINE void Remove(const size_t index) {
        CELL_ASSERT(index < this->count);

        if (index < this->count - 1) {
            System::CopyMemory<K>(this->keys + index, this->keys + index + 1, this->count - index);
            System::CopyMemory<V>(this->values + index, this->values + index + 1, this->count - index);
        }

        System::ReallocateMemory<K>(&this->keys, --this->count);
        System::ReallocateMemory<V>(&this->values, this->count);
    }

    // Removes the given key and its value at the first found index which equals it.
    CELL_INLINE bool Remove(const K& key) {
        CELL_ASSERT(this->count > 0);

        for (size_t i = 0; i < this->count; i++) {
            if (this->keys[i] == key) {
                Remove(i);
                return true;
            }
        }

        return false;
    }

    // Resets the entire list to be empty.
    CELL_INLINE void Reset() {
        this->count = 0;

        System::FreeMemory(this->keys);
        System::FreeMemory(this->values);
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
            this->keys = System::AllocateMemory<K>(count);
            this->values = System::AllocateMemory<V>(count);
        } else {
            System::ReallocateMemory<K>(&this->keys, count);
            System::ReallocateMemory<V>(&this->values, count);
        }

        this->count = count;
    }

    // Checks if the dictionary contains a specific key, and if it does, returns its index.
    CELL_NODISCARD CELL_INLINE Optional<size_t> Has(const K& key) {
        for (size_t i = 0; i < this->count; i++) {
            if (this->keys[i] == key) {
                return i;
            }
        }

        return None<size_t>();
    }

    // Retrieves the element with the specified key.
    CELL_NODISCARD CELL_INLINE V& GetValue(const K& key) {
        Optional<size_t> position = this->Has(key);
        CELL_ASSERT(position.IsValid());

        return this->GetValue(position.Unwrap());
    }

    // Retrieves the key at the specific index.
    CELL_NODISCARD CELL_INLINE K& GetKey(const size_t index) {
        CELL_ASSERT(index < this->count);

        return this->keys[index];
    }

    // Retrieves the value at the specific index.
    CELL_NODISCARD CELL_INLINE V& GetValue(const size_t index) {
        CELL_ASSERT(index < this->count);

        return this->values[index];
    }

    // Sets the value at the given index.
    CELL_INLINE void Set(const size_t index, const V& value) {
        this->values[index] = value;
    }

    CELL_NODISCARD K& operator [] (const size_t index) {
        CELL_ASSERT(index < this->count);

        return this->keys[index];
    }

    // begin operator for loops.
    CELL_NODISCARD CELL_INLINE K* begin() {
        return &this->keys[0];
    }

    // end operator for loops.
    CELL_NODISCARD CELL_INLINE K* end() {
        return &this->keys[this->count];
    }

private:
    K* keys;
    V* values;

    size_t count;
};

}
