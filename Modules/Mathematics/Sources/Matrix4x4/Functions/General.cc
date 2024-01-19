// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Shared.hh"

#include <Cell/Mathematics/Matrix4x4.hh>

#ifdef CELL_MODULES_MATHEMATICS_HARDWARE_ACCELERATE_X86
#define shuffle(v, z, y, x, w) _mm_shuffle_ps(v, v, _MM_SHUFFLE(z, y, x, w))
#endif

namespace Cell::Mathematics {

Matrix4x4 Matrix4x4::SetToIdentity() {
    this->data[0][0] = 1.f;
    this->data[0][1] = 0.f;
    this->data[0][2] = 0.f;
    this->data[0][3] = 0.f;

    this->data[1][0] = 0.f;
    this->data[1][1] = 1.f;
    this->data[1][2] = 0.f;
    this->data[1][3] = 0.f;

    this->data[2][0] = 0.f;
    this->data[2][1] = 0.f;
    this->data[2][2] = 1.f;
    this->data[2][3] = 0.f;

    this->data[3][0] = 0.f;
    this->data[3][1] = 0.f;
    this->data[3][2] = 0.f;
    this->data[3][3] = 1.f;

    return *this;
}

Matrix4x4 Matrix4x4::Invert() {
#ifdef CELL_MODULES_MATHEMATICS_HARDWARE_ACCELERATE_X86
    int128_t x8 = _mm_set_ps(-0.f, 0.f, -0.f, 0.f);
    int128_t x9 = shuffle(x8, 2, 1, 2, 1);

    int128_t r0 = _mm_load_ps(this->data[0]);
    int128_t r1 = _mm_load_ps(this->data[1]);
    int128_t r2 = _mm_load_ps(this->data[2]);
    int128_t r3 = _mm_load_ps(this->data[3]);

    int128_t x0 = _mm_movehl_ps(r3, r2);
    int128_t x3 = _mm_movelh_ps(r2, r3);
    int128_t x1 = shuffle(x0, 1, 3, 3, 3);
    int128_t x2 = shuffle(x0, 0, 2, 2, 2);
    int128_t x4 = shuffle(x3, 1, 3, 3, 3);
    int128_t x7 = shuffle(x3, 0, 2, 2, 2);

    int128_t x6 = _mm_shuffle_ps(r2, r1, _MM_SHUFFLE(0, 0, 0, 0));
    int128_t x5 = _mm_shuffle_ps(r2, r1, _MM_SHUFFLE(1, 1, 1, 1));

    x3 = _mm_shuffle_ps(r2, r1, _MM_SHUFFLE(2, 2, 2, 2));
    x0 = _mm_shuffle_ps(r2, r1, _MM_SHUFFLE(3, 3, 3, 3));

    int128_t t0 = _mm_mul_ps(x3, x1);
    int128_t t1 = _mm_mul_ps(x5, x1);
    int128_t t2 = _mm_mul_ps(x5, x2);
    int128_t t3 = _mm_mul_ps(x6, x1);
    int128_t t4 = _mm_mul_ps(x6, x2);
    int128_t t5 = _mm_mul_ps(x6, x4);

    t0 = FusedNegativeMultiplyAdd(x2, x0, t0);
    t1 = FusedNegativeMultiplyAdd(x4, x0, t1);
    t2 = FusedNegativeMultiplyAdd(x4, x3, t2);
    t3 = FusedNegativeMultiplyAdd(x7, x0, t3);
    t4 = FusedNegativeMultiplyAdd(x7, x3, t4);
    t5 = FusedNegativeMultiplyAdd(x7, x5, t5);

    x4 = _mm_movelh_ps(r0, r1);
    x5 = _mm_movehl_ps(r1, r0);

    x0 = shuffle(x4, 0, 0, 0, 2);
    x1 = shuffle(x4, 1, 1, 1, 3);
    x2 = shuffle(x5, 0, 0, 0, 2);
    x3 = shuffle(x5, 1, 1, 1, 3);

    int128_t v2 = _mm_mul_ps(x0, t1);
    int128_t v1 = _mm_mul_ps(x0, t0);
    int128_t v3 = _mm_mul_ps(x0, t2);
    int128_t v0 = _mm_mul_ps(x1, t0);

    v2 = FusedNegativeMultiplyAdd(x1, t3, v2);
    v3 = FusedNegativeMultiplyAdd(x1, t4, v3);
    v0 = FusedNegativeMultiplyAdd(x2, t1, v0);
    v1 = FusedNegativeMultiplyAdd(x2, t3, v1);

    v3 = FusedMultiplyAdd(x2, t5, v3);
    v0 = FusedMultiplyAdd(x3, t2, v0);
    v2 = FusedMultiplyAdd(x3, t5, v2);
    v1 = FusedMultiplyAdd(x3, t4, v1);

    v0 = _mm_xor_ps(v0, x8);
    v2 = _mm_xor_ps(v2, x8);
    v1 = _mm_xor_ps(v1, x9);
    v3 = _mm_xor_ps(v3, x9);

    x0 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(0, 0, 0, 0));
    x1 = _mm_shuffle_ps(v2, v3, _MM_SHUFFLE(0, 0, 0, 0));
    x0 = _mm_shuffle_ps(x0, x1, _MM_SHUFFLE(2, 0, 2, 0));

    int128_t v = _mm_mul_ps(x0, r0);
    x0 = _mm_add_ps(v, shuffle(v, 0, 1, 2, 3));
    x0 = _mm_add_ps(x0, shuffle(x0, 1, 0, 0, 1));
    x0 = _mm_rcp_ps(x0);

    _mm_store_ps(this->data[0], _mm_mul_ps(v0, x0));
    _mm_store_ps(this->data[1], _mm_mul_ps(v1, x0));
    _mm_store_ps(this->data[2], _mm_mul_ps(v2, x0));
    _mm_store_ps(this->data[3], _mm_mul_ps(v3, x0));
#else
    float a = this->data[0][0], b = this->data[0][1], c = this->data[0][2], d = this->data[0][3],
        e = this->data[1][0], f = this->data[1][1], g = this->data[1][2], h = this->data[1][3],
        i = this->data[2][0], j = this->data[2][1], k = this->data[2][2], l = this->data[2][3],
        m = this->data[3][0], n = this->data[3][1], o = this->data[3][2], p = this->data[3][3];

    float block[6] = {
        k * p - o * l,
        j * p - n * l,
        j * o - n * k,
        i * p - m * l,
        i * o - m * k,
        i * n - m * j
    };

    this->data[0][0] = f * block[0] - g * block[1] + h * block[2];
    this->data[0][1] = -(e * block[0] - g * block[3] + h * block[4]);
    this->data[0][2] = e * block[1] - f * block[3] + h * block[5];
    this->data[0][3] = -(e * block[2] - f * block[4] + g * block[5]);

    this->data[1][0] = -(b * block[0] - c * block[1] + d * block[2]);
    this->data[1][1] = a * block[0] - c * block[3] + d * block[4];
    this->data[1][2] = -(a * block[1] - b * block[3] + d * block[5]);
    this->data[1][3] = a * block[2] - b * block[4] + c * block[5];

    block[0] = g * p - o * h;
    block[1] = f * p - n * h;
    block[2] = f * o - n * g;
    block[3] = e * p - m * h;
    block[4] = e * o - m * g;
    block[5] = e * n - m * f;

    this->data[2][0] = b * block[0] - c * block[1] + d * block[2];
    this->data[2][1] = -(a * block[0] - c * block[3] + d * block[4]);
    this->data[2][2] = a * block[1] - b * block[3] + d * block[5];
    this->data[2][3] = -(a * block[2] - b * block[4] + c * block[5]);

    block[0] = g * l - k * h;
    block[1] = f * l - j * h;
    block[2] = f * k - j * g;
    block[3] = e * l - i * h;
    block[4] = e * k - i * g;
    block[5] = e * j - i * f;

    this->data[3][0] = -(b * block[0] - c * block[1] + d * block[2]);
    this->data[3][1] = a * block[0] - c * block[3] + d * block[4];
    this->data[3][2] = -(a * block[1] - b * block[3] + d * block[5]);
    this->data[3][3] = a * block[2] - b * block[4] + c * block[5];

    float det = 1.f / (a * this->data[0][0] + b * this->data[1][0] + c * this->data[2][0] + d * this->data[3][0]);
    this->Multiply(det);
#endif

    return *this;
}

// TODO: SIMD for Addition, Subtraction and Multiplication

Matrix4x4 Matrix4x4::Add(const float scalar) {
    this->data[0][0] += scalar;
    this->data[0][1] += scalar;
    this->data[0][2] += scalar;
    this->data[0][3] += scalar;

    this->data[1][0] += scalar;
    this->data[1][1] += scalar;
    this->data[1][2] += scalar;
    this->data[1][3] += scalar;

    this->data[2][0] += scalar;
    this->data[2][1] += scalar;
    this->data[2][2] += scalar;
    this->data[2][3] += scalar;

    this->data[3][0] += scalar;
    this->data[3][1] += scalar;
    this->data[3][2] += scalar;
    this->data[3][3] += scalar;

    return *this;
}

Matrix4x4 Matrix4x4::Add(const Matrix4x4 matrix) {
    Matrix4x4 old = *this;

    this->data[0][0] = old.data[0][0] + matrix.data[0][0];
    this->data[0][2] = old.data[0][1] + matrix.data[0][1];
    this->data[0][2] = old.data[0][2] + matrix.data[0][2];
    this->data[0][3] = old.data[0][3] + matrix.data[0][3];

    this->data[1][0] = old.data[1][0] + matrix.data[1][0];
    this->data[1][2] = old.data[1][1] + matrix.data[1][1];
    this->data[1][2] = old.data[1][2] + matrix.data[1][2];
    this->data[1][3] = old.data[1][3] + matrix.data[1][3];

    this->data[2][0] = old.data[2][0] + matrix.data[2][0];
    this->data[2][2] = old.data[2][1] + matrix.data[2][1];
    this->data[2][2] = old.data[2][2] + matrix.data[2][2];
    this->data[2][3] = old.data[2][3] + matrix.data[2][3];

    this->data[3][0] = old.data[3][0] + matrix.data[3][0];
    this->data[3][2] = old.data[3][1] + matrix.data[3][1];
    this->data[3][2] = old.data[3][2] + matrix.data[3][2];
    this->data[3][3] = old.data[3][3] + matrix.data[3][3];

    return *this;
}

Matrix4x4 Matrix4x4::Subtract(const float scalar) {
    this->data[0][0] -= scalar;
    this->data[0][1] -= scalar;
    this->data[0][2] -= scalar;
    this->data[0][3] -= scalar;

    this->data[1][0] -= scalar;
    this->data[1][1] -= scalar;
    this->data[1][2] -= scalar;
    this->data[1][3] -= scalar;

    this->data[2][0] -= scalar;
    this->data[2][1] -= scalar;
    this->data[2][2] -= scalar;
    this->data[2][3] -= scalar;

    this->data[3][0] -= scalar;
    this->data[3][1] -= scalar;
    this->data[3][2] -= scalar;
    this->data[3][3] -= scalar;

    return *this;
}

Matrix4x4 Matrix4x4::Subtract(const Matrix4x4 matrix) {
    Matrix4x4 old = *this;

    this->data[0][0] = old.data[0][0] - matrix.data[0][0];
    this->data[0][2] = old.data[0][1] - matrix.data[0][1];
    this->data[0][2] = old.data[0][2] - matrix.data[0][2];
    this->data[0][3] = old.data[0][3] - matrix.data[0][3];

    this->data[1][0] = old.data[1][0] - matrix.data[1][0];
    this->data[1][2] = old.data[1][1] - matrix.data[1][1];
    this->data[1][2] = old.data[1][2] - matrix.data[1][2];
    this->data[1][3] = old.data[1][3] - matrix.data[1][3];

    this->data[2][0] = old.data[2][0] - matrix.data[2][0];
    this->data[2][2] = old.data[2][1] - matrix.data[2][1];
    this->data[2][2] = old.data[2][2] - matrix.data[2][2];
    this->data[2][3] = old.data[2][3] - matrix.data[2][3];

    this->data[3][0] = old.data[3][0] - matrix.data[3][0];
    this->data[3][2] = old.data[3][1] - matrix.data[3][1];
    this->data[3][2] = old.data[3][2] - matrix.data[3][2];
    this->data[3][3] = old.data[3][3] - matrix.data[3][3];

    return *this;
}

Matrix4x4 Matrix4x4::Multiply(const float scalar) {
    this->data[0][0] *= scalar;
    this->data[0][1] *= scalar;
    this->data[0][2] *= scalar;
    this->data[0][3] *= scalar;

    this->data[1][0] *= scalar;
    this->data[1][1] *= scalar;
    this->data[1][2] *= scalar;
    this->data[1][3] *= scalar;

    this->data[2][0] *= scalar;
    this->data[2][1] *= scalar;
    this->data[2][2] *= scalar;
    this->data[2][3] *= scalar;

    this->data[3][0] *= scalar;
    this->data[3][1] *= scalar;
    this->data[3][2] *= scalar;
    this->data[3][3] *= scalar;

    return *this;
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4 matrix) {
    Matrix4x4 old = *this;

    this->data[0][0] = old.data[0][0] * matrix.data[0][0] + old.data[1][0] * matrix.data[0][1] + old.data[2][0] * matrix.data[0][2] + old.data[3][0] * matrix.data[0][3];
    this->data[0][1] = old.data[0][1] * matrix.data[0][0] + old.data[1][1] * matrix.data[0][1] + old.data[2][1] * matrix.data[0][2] + old.data[3][1] * matrix.data[0][3];
    this->data[0][2] = old.data[0][2] * matrix.data[0][0] + old.data[1][2] * matrix.data[0][1] + old.data[2][2] * matrix.data[0][2] + old.data[3][2] * matrix.data[0][3];
    this->data[0][3] = old.data[0][3] * matrix.data[0][0] + old.data[1][3] * matrix.data[0][1] + old.data[2][3] * matrix.data[0][2] + old.data[3][3] * matrix.data[0][3];

    this->data[1][0] = old.data[0][0] * matrix.data[1][0] + old.data[1][0] * matrix.data[1][1] + old.data[2][0] * matrix.data[1][2] + old.data[3][0] * matrix.data[1][3];
    this->data[1][1] = old.data[0][1] * matrix.data[1][0] + old.data[1][1] * matrix.data[1][1] + old.data[2][1] * matrix.data[1][2] + old.data[3][1] * matrix.data[1][3];
    this->data[1][2] = old.data[0][2] * matrix.data[1][0] + old.data[1][2] * matrix.data[1][1] + old.data[2][2] * matrix.data[1][2] + old.data[3][2] * matrix.data[1][3];
    this->data[1][3] = old.data[0][3] * matrix.data[1][0] + old.data[1][3] * matrix.data[1][1] + old.data[2][3] * matrix.data[1][2] + old.data[3][3] * matrix.data[1][3];

    this->data[2][0] = old.data[0][0] * matrix.data[2][0] + old.data[1][0] * matrix.data[2][1] + old.data[2][0] * matrix.data[2][2] + old.data[3][0] * matrix.data[2][3];
    this->data[2][1] = old.data[0][1] * matrix.data[2][0] + old.data[1][1] * matrix.data[2][1] + old.data[2][1] * matrix.data[2][2] + old.data[3][1] * matrix.data[2][3];
    this->data[2][2] = old.data[0][2] * matrix.data[2][0] + old.data[1][2] * matrix.data[2][1] + old.data[2][2] * matrix.data[2][2] + old.data[3][2] * matrix.data[2][3];
    this->data[2][3] = old.data[0][3] * matrix.data[2][0] + old.data[1][3] * matrix.data[2][1] + old.data[2][3] * matrix.data[2][2] + old.data[3][3] * matrix.data[2][3];

    this->data[3][0] = old.data[0][0] * matrix.data[3][0] + old.data[1][0] * matrix.data[3][1] + old.data[2][0] * matrix.data[3][2] + old.data[3][0] * matrix.data[3][3];
    this->data[3][1] = old.data[0][1] * matrix.data[3][0] + old.data[1][1] * matrix.data[3][1] + old.data[2][1] * matrix.data[3][2] + old.data[3][1] * matrix.data[3][3];
    this->data[3][2] = old.data[0][2] * matrix.data[3][0] + old.data[1][2] * matrix.data[3][1] + old.data[2][2] * matrix.data[3][2] + old.data[3][2] * matrix.data[3][3];
    this->data[3][3] = old.data[0][3] * matrix.data[3][0] + old.data[1][3] * matrix.data[3][1] + old.data[2][3] * matrix.data[3][2] + old.data[3][3] * matrix.data[3][3];

    return *this;
}

}
