// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Memory/Allocator.hh>
#include <Cell/Memory/Block.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Utilities {

// Block reading utility.
class Reader : public Object {
public:
    CELL_FUNCTION_TEMPLATE Reader(const Memory::IBlock& ref) : ref(ref) { }

    template <typename T> CELL_FUNCTION_TEMPLATE T Read(const bool advance = true) {
        CELL_ASSERT(this->offset < ref.GetSize());

        this->bitByte       = 0;
        this->bitsRemaining = 0;

        const T value = *(T*)(this->ref.AsBytes() + this->offset);
        if (advance) {
            this->offset += sizeof(T);
        }

        return value;
    }

    CELL_FUNCTION_TEMPLATE uint8_t ReadBits(const uint8_t bits, const bool advance = true) {
        CELL_ASSERT(bits < 9);
        CELL_ASSERT(this->offset < ref.GetSize());

        uint8_t value = 0;

        for (uint8_t i = 0; i < bits; i++) {
            if (this->bitsRemaining == 0) {
                this->bitByte       = this->Read<uint8_t>(advance);
                this->bitsRemaining = 8;
            }

            value |= (this->bitByte & 1) << i;

            this->bitByte >>= 1;
            this->bitsRemaining--;
        }

        return value;
    }

    template <size_t S> CELL_FUNCTION_TEMPLATE void ReadArray(uint8_t block[S], const bool advance = true) {
        CELL_ASSERT(this->offset < ref.GetSize());

        this->bitByte       = 0;
        this->bitsRemaining = 0;

        Memory::Copy(block, this->ref.AsBytes() + this->offset, S);
        if (advance) {
            this->offset += S;
        }
    }

    CELL_FUNCTION_TEMPLATE void ReadBytes(uint8_t* block, const size_t size, const bool advance = true) {
        CELL_ASSERT(this->offset < ref.GetSize());

        this->bitByte       = 0;
        this->bitsRemaining = 0;

        Memory::Copy(block, this->ref.AsBytes() + this->offset, size);
        if (advance) {
            this->offset += size;
        }
    }

    CELL_FUNCTION_TEMPLATE size_t GetCurrentOffset() const {
        return this->offset;
    }

private:
    const Memory::IBlock& ref;

    size_t  offset        = 0;
    uint8_t bitByte       = 0;
    uint8_t bitsRemaining = 0;
};

}
