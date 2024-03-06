// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Memory.hh>
#include <Cell/Utilities/Concepts.hh>

template <typename T> struct RemovedReferenceMap { using type = T; };
template <typename T> struct RemovedReferenceMap<T&> { using type = T; };
template <typename T> using RemovedReference = RemovedReferenceMap<T>::type;

template <typename T> constexpr RemovedReference<T>&& MoveObject(T&& t) noexcept { return (RemovedReference<T>&&)(t); }

using namespace Cell;

template <typename T, typename R> class Wrapped2 : public Object {
public:
    CELL_FUNCTION_INTERNAL CELL_INLINE constexpr Wrapped2(T&& object) requires Utilities::IsClassType<T> : object(MoveObject(object)), isValid(true) { }
    CELL_FUNCTION_INTERNAL CELL_INLINE constexpr Wrapped2(T object) requires (!Utilities::IsClassType<T>) : object(object), isValid(true) { }

    CELL_FUNCTION_INTERNAL CELL_INLINE constexpr Wrapped2(R result) : result(result), isValid(false) { }
    CELL_FUNCTION_INTERNAL CELL_INLINE constexpr ~Wrapped2() { }

    CELL_NODISCARD CELL_FUNCTION_INTERNAL CELL_INLINE T Unwrap() requires Utilities::IsClassType<T> {
        return MoveObject(this->object);
    }

    CELL_NODISCARD CELL_FUNCTION_INTERNAL CELL_INLINE T Unwrap() requires (!Utilities::IsClassType<T>) {
        return this->object;
    }

    CELL_NODISCARD CELL_FUNCTION_INTERNAL CELL_INLINE R Result() const {
        return this->result;
    }

    CELL_NODISCARD CELL_FUNCTION_INTERNAL CELL_INLINE bool IsValid() const {
        return this->isValid;
    }

    // Returns whether this wrapped result failed, which means the object it holds is invalid.
    CELL_NODISCARD CELL_INLINE operator bool() const {
        return this->isValid;
    }

private:
    struct Dummy { constexpr Dummy() { }; };
    CELL_STATIC_ASSERT(!__is_trivially_constructible(Dummy));

    union {
        Dummy _dummy;
        T object;
    };

    R result = R::Success;
    bool isValid;
};

enum class Result : uint8_t {
    Success,
    Failure
};

#define NON_COPYABLE(T) \
public: \
    CELL_FUNCTION_INTERNAL constexpr T(T& other) = delete; \
    CELL_FUNCTION_INTERNAL constexpr T(T&& other) { other.wasMoved = true; this->MoveContents(other); } \
    CELL_FUNCTION ~T() { \
        if (!this->wasMoved) { \
            this->Release(); \
        } \
        \
    } \
 \
protected: \
    bool wasMoved = false;

class Test {
public:
    CELL_FUNCTION_INTERNAL static Wrapped2<Test, Result> New() {
        uint8_t* block = System::AllocateMemory<uint8_t>(5);

        return Test(block);
    }

    CELL_FUNCTION_INTERNAL void DoStuff() {
        this->block[3] += 2;
    }

    CELL_FUNCTION_INTERNAL Wrapped2<uint8_t, Result> GetStuff() {
        return this->block[3];
    }

private:
    CELL_FUNCTION_INTERNAL CELL_INLINE Test(uint8_t* b) : block(b) { }

    void MoveContents(Test& other) {
        this->block = other.block;
    }

    void Release() {
        System::FreeMemory(this->block);
    }

    uint8_t* block;

    NON_COPYABLE(Test)
};

CELL_FUNCTION_INTERNAL void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    Wrapped2<Test, Result> testResult = Test::New();
    Test test = testResult.Unwrap();

    test.DoStuff();
    System::Log("%", test.GetStuff().Unwrap());
}
