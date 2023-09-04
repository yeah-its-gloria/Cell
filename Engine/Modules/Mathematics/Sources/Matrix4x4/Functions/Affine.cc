// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Shared.hh"

#include <Cell/Mathematics/Matrix4x4.hh>
#include <Cell/Mathematics/Vector4.hh>
#include <Cell/System/Panic.hh>

#include <math.h>

#ifdef CELL_MODULES_MATHEMATICS_HARDWARE_ACCELERATE_X86
#define splat(x, lane) _mm_shuffle_ps(x, x, _MM_SHUFFLE(lane, lane, lane, lane))
#endif

namespace Cell::Mathematics {

CELL_INLINE void MultiplyAndAdd(Vector4* out, const float value, const Vector4 data) {
#ifdef CELL_MODULES_MATHEMATICS_HARDWARE_ACCELERATE_X86
    float out1[4] = { 0.f, 0.f, 0.f, 0.f };
    float datdat1[4] = { data.x, data.y, data.z, data.w };

    _mm_store_ps(out1, FusedMultiplyAdd(_mm_load_ps((float*)datdat1), _mm_set1_ps(value), _mm_load_ps((float*)out1)));

    out->x += out1[0];
    out->y += out1[1];
    out->z += out1[2];
    out->w += out1[3];
#else
    out->x += data.x * value;
    out->y += data.y * value;
    out->z += data.z * value;
    out->w += data.w * value;
#endif
}

Matrix4x4 Matrix4x4::Translate(const Vector3 position) {
#ifdef CELL_MODULES_MATHEMATICS_HARDWARE_ACCELERATE_X86
    const int128_t m0 = _mm_load_ps((float*)this->data[0]);
    const int128_t m1 = _mm_load_ps((float*)this->data[1]);
    const int128_t m2 = _mm_load_ps((float*)this->data[2]);
    const int128_t m3 = _mm_load_ps((float*)this->data[3]);

    _mm_store_ps((float*)this->data[3],
                 FusedMultiplyAdd(m0, _mm_set1_ps(position.x),
                                  FusedMultiplyAdd(m1, _mm_set1_ps(position.y),
                                                   FusedMultiplyAdd(m2, _mm_set1_ps(position.z), m3))));
#else
    MultiplyAndAdd((Vector4*)this->data[0], position.x, *(Vector4*)this->data[3]);
    MultiplyAndAdd((Vector4*)this->data[1], position.y, *(Vector4*)this->data[3]);
    MultiplyAndAdd((Vector4*)this->data[2], position.z, *(Vector4*)this->data[3]);
#endif

    return *this;
}

Matrix4x4 Matrix4x4::Rotate(const float angle, const Vector3 axis) {
    const float cosine = cosf(angle);

    const Vector3 axisNormal = axis.Normalize();
    const Vector3 v = axisNormal * (1.f - cosine);
    const Vector3 vs = axisNormal * sinf(angle);

    const Vector3 out_data_1 = axisNormal * v.x;
    const Vector3 out_data_2 = axisNormal * v.y;
    const Vector3 out_data_3 = axisNormal * v.z;

    Matrix4x4 rotation;

    rotation.data[0][0] = out_data_1.x + cosine;
    rotation.data[0][1] = out_data_1.y + vs.z;
    rotation.data[0][2] = out_data_1.z - vs.y;
    rotation.data[0][3] = 0.f;

    rotation.data[1][0] = out_data_2.x - vs.z;
    rotation.data[1][1] = out_data_2.y + cosine;
    rotation.data[1][2] = out_data_2.z + vs.x;
    rotation.data[1][3] = 0.f;

    rotation.data[2][0] = out_data_3.x + vs.y;
    rotation.data[2][1] = out_data_3.y - vs.x;
    rotation.data[2][2] = out_data_3.z + cosine;
    rotation.data[2][3] = 0.f;

    rotation.data[3][0] = 0.f;
    rotation.data[3][1] = 0.f;
    rotation.data[3][2] = 0.f;
    rotation.data[3][3] = 1.f;

#ifdef CELL_MODULES_MATHEMATICS_HARDWARE_ACCELERATE_X86
    const int128_t rot0 = _mm_load_ps(rotation.data[0]);
    const int128_t rot1 = _mm_load_ps(rotation.data[1]);
    const int128_t rot2 = _mm_load_ps(rotation.data[2]);

    int128_t index = _mm_load_ps(this->data[0]);
    int128_t val0 = _mm_mul_ps(splat(rot0, 0), index);
    int128_t val1 = _mm_mul_ps(splat(rot1, 0), index);
    int128_t val2 = _mm_mul_ps(splat(rot2, 0), index);

    index = _mm_load_ps(this->data[1]);
    val0 = FusedMultiplyAdd(splat(rot0, 1), index, val0);
    val1 = FusedMultiplyAdd(splat(rot1, 1), index, val1);
    val2 = FusedMultiplyAdd(splat(rot2, 1), index, val2);

    index = _mm_load_ps(this->data[2]);
    val0 = FusedMultiplyAdd(splat(rot0, 2), index, val0);
    val1 = FusedMultiplyAdd(splat(rot1, 2), index, val1);
    val2 = FusedMultiplyAdd(splat(rot2, 2), index, val2);

    _mm_store_ps(this->data[0], val0);
    _mm_store_ps(this->data[1], val1);
    _mm_store_ps(this->data[2], val2);
#else
    const float dat00 = this->data[0][0];
    const float dat01 = this->data[0][1];
    const float dat02 = this->data[0][2];
    const float dat03 = this->data[0][3];

    const float dat10 = this->data[1][0];
    const float dat11 = this->data[1][1];
    const float dat12 = this->data[1][2];
    const float dat13 = this->data[1][3];

    const float dat20 = this->data[2][0];
    const float dat21 = this->data[2][1];
    const float dat22 = this->data[2][2];
    const float dat23 = this->data[2][3];

    const float rot00 = rotation.data[0][0];
    const float rot01 = rotation.data[0][1];
    const float rot02 = rotation.data[0][2];

    const float rot10 = rotation.data[1][0];
    const float rot11 = rotation.data[1][1];
    const float rot12 = rotation.data[1][2];

    const float rot20 = rotation.data[2][0];
    const float rot21 = rotation.data[2][1];
    const float rot22 = rotation.data[2][2];

    this->data[0][0] = dat00 * rot00 + dat10 * rot01 + dat20 * rot02;
    this->data[0][1] = dat01 * rot00 + dat11 * rot01 + dat21 * rot02;
    this->data[0][2] = dat02 * rot00 + dat12 * rot01 + dat22 * rot02;
    this->data[0][3] = dat03 * rot00 + dat13 * rot01 + dat23 * rot02;

    this->data[1][0] = dat00 * rot10 + dat10 * rot11 + dat20 * rot12;
    this->data[1][1] = dat01 * rot10 + dat11 * rot11 + dat21 * rot12;
    this->data[1][2] = dat02 * rot10 + dat12 * rot11 + dat22 * rot12;
    this->data[1][3] = dat03 * rot10 + dat13 * rot11 + dat23 * rot12;

    this->data[2][0] = dat00 * rot20 + dat10 * rot21 + dat20 * rot22;
    this->data[2][1] = dat01 * rot20 + dat11 * rot21 + dat21 * rot22;
    this->data[2][2] = dat02 * rot20 + dat12 * rot21 + dat22 * rot22;
    this->data[2][3] = dat03 * rot20 + dat13 * rot21 + dat23 * rot22;
#endif

    return *this;
}

Matrix4x4 Matrix4x4::Scale(const Vector3 scale) {
    const Vector4 x = *(Vector4*)this->data[0] * scale.x;
    const Vector4 y = *(Vector4*)this->data[0] * scale.y;
    const Vector4 z = *(Vector4*)this->data[0] * scale.z;

    this->data[0][0] = x.x;
    this->data[0][1] = x.y;
    this->data[0][2] = x.z;
    this->data[0][3] = 0.f;

    this->data[1][0] = y.x;
    this->data[1][1] = y.y;
    this->data[1][2] = y.z;
    this->data[1][3] = 0.f;

    this->data[2][0] = z.x;
    this->data[2][1] = z.y;
    this->data[2][2] = z.z;
    this->data[2][3] = 0.f;

    return *this;
}

}
