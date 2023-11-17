// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/Collection/Enumerable.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Collection {

// Key-value dictionary enumerable.
template <typename K, typename V> class Dictionary : public IEnumerable<V> {
public:
    // Creates a dictionary with the given number of empty key-value pairs.
    // By default, it creates an entirely empty pair block.
    CELL_INLINE explicit Dictionary(const size_t size = 0) : count(size) {
        if (this->count > 0) {
            this->pairBlock = System::AllocateMemory<pair>(this->count);
        }
    }

    // Creates a dictionary with an initial set.
    CELL_INLINE Dictionary(const K& key, const V& value) : count(1) {
        this->pairBlock = System::AllocateMemory<pair>();

        System::CopyMemory<K>(&this->pairBlock[0].key, &key);
        System::CopyMemory<V>(&this->pairBlock[0].value, &value);
    }

    // Creates a dictionary by copying the elements of another.
    CELL_INLINE Dictionary(const Dictionary<K, V>& dictionary) : count(0), pairBlock(nullptr) {
        if (dictionary.count == 0) {
            return;
        }

        this->count = dictionary.count;
        this->pairBlock = System::AllocateMemory<pair>(this->count);

        System::CopyMemory<pair>(this->pairBlock, dictionary.pairBlock, this->count);
    }

    // Frees all pairs and destructs the dictionary.
    CELL_INLINE ~Dictionary() {
        if (this->pairBlock != nullptr) {
            System::FreeMemory(this->pairBlock);
        }
    }

    // Appends a key-value pair to the end of the list.
    CELL_INLINE void Append(const K& key, const V& value) {
        if (this->count == 0) {
            this->pairBlock = System::AllocateMemory<pair>();
            this->count++;
        } else {
            System::ReallocateMemory<pair>(&this->pairBlock, this->count + 1);
        }

        System::CopyMemory<K>(&this->pairBlock[this->count].key, &key);
        System::CopyMemory<V>(&this->pairBlock[this->count].value, &value);

        this->count++;
    }

    // Removes the pair at the given index.
    CELL_INLINE void Remove(const size_t index) {
        CELL_ASSERT(index < this->count);

        if (index < this->count - 1) {
            System::CopyMemory<pair>(this->pairBlock + index, this->pairBlock + index + 1, this->count - index);
        }

        System::ReallocateMemory<pair>(&this->pairBlock, --this->count);
    }

    // Removes the key-value pair with the first matching key.
    CELL_INLINE bool Remove(const K& key) {
        CELL_ASSERT(this->count > 0);

        for (size_t i = 0; i < this->count; i++) {
            if (this->pairBlock[i].key == key) {
                Remove(i);
                return true;
            }
        }

        return false;
    }

    // Resets the dictionary by removing all contained pairs.
    CELL_INLINE void Reset() {
        this->count = 0;

        if (this->pairBlock != nullptr) {
            System::FreeMemory(this->pairBlock);
            this->pairBlock = nullptr;
        }
    }

    // Returns the number of stored pairs.
    CELL_NODISCARD CELL_INLINE size_t GetCount() const override {
        return this->count;
    }

    // Changes the number of stored pairs in the storage block.
    // In case this size is smaller, the dictionary is cut from its end.
    // If the size is bigger, empty pairs are added to the end.
    CELL_INLINE void SetCount(const size_t count) {
        CELL_ASSERT(count > 0);

        if (this->count == 0) {
            this->pairBlock = System::AllocateMemory<pair>(count);
        } else {
            System::ReallocateMemory<pair>(&this->pairBlock, count);
        }

        this->count = count;
    }

    // Checks for a matching key in the pair list, and returns its first index if found.
    CELL_NODISCARD CELL_INLINE Optional<size_t> Has(const K& key) {
        for (size_t i = 0; i < this->count; i++) {
            if (this->pairBlock[i].key == key) {
                return i;
            }
        }

        return None<size_t>();
    }

    // Returns the value in the first found pair matching the given key.
    CELL_NODISCARD CELL_INLINE V& GetValue(const K& key) {
        Optional<size_t> position = this->Has(key);
        CELL_ASSERT(position.IsValid());

        return this->GetValue(position.Unwrap());
    }

    // Returns the key at the given index.
    CELL_NODISCARD CELL_INLINE K& GetKey(const size_t index) {
        CELL_ASSERT(index < this->count);

        return this->pairBlock[index].key;
    }

    // Returns the value at the given index.
    CELL_NODISCARD CELL_INLINE V& GetValue(const size_t index) {
        CELL_ASSERT(index < this->count);

        return this->pairBlock[index].value;
    }

    // Sets the value at the given index.
    CELL_INLINE void Set(const size_t index, const V& value) {
        this->pairBlock[index].value = value;
    }

    // Gets the value at the given index.
    // Meant for the enumerator interface.
    CELL_NODISCARD V& operator [] (const size_t index) override {
        return this->GetValue(index);
    }

    // Begin operator for the enumerator interface.
    // Returns the address of the value in the first pair.
    CELL_NODISCARD CELL_INLINE V* begin() override {
        return &this->pairBlock[0].value;
    }

    // End operator for the enumerator interface.
    // Returns the address of the value in the last pair.
    CELL_NODISCARD CELL_INLINE V* end() override {
        return &this->pairBlock[this->count].value;
    }

private:
    struct pair { K key; V value; };

    pair* pairBlock;
    size_t count;
};

}