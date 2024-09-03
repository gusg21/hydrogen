#include "core/math/quaternion.h"

#include <stdio.h>

float h_core::math::Quaternion::getLength() const {
    return h_core::math::Quaternion::getLength(*this);
}

h_core::math::Quaternion h_core::math::Quaternion::normalize(
    h_core::math::Quaternion a) {
    float length = h_core::math::Quaternion::getLength(a);
    if (length <= 0.f) { return h_core::math::Quaternion {}; }

    float inverseLength = 1.f / length;
    // clang-format off
    return h_core::math::Quaternion { a.x * inverseLength, 
                                      a.y * inverseLength,
                                      a.z * inverseLength,
                                      a.w * inverseLength };
    // clang-format on
}

float h_core::math::Quaternion::dot(
    h_core::math::Quaternion a, h_core::math::Quaternion b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float h_core::math::Quaternion::getLength(h_core::math::Quaternion a) {
    return ::sqrt(h_core::math::Quaternion::dot(a, a));
}

h_core::math::Quaternion h_core::math::Quaternion::fromEuler(Vector3 eulers) {
    h_core::math::Quaternion quat {};

    float cx = ::cos(eulers.x * 0.5f);
    float sx = ::sin(eulers.x * 0.5f);
    float cy = ::cos(eulers.y * 0.5f);
    float sy = ::sin(eulers.y * 0.5f);
    float cz = ::cos(eulers.z * 0.5f);
    float sz = ::sin(eulers.z * 0.5f);

    quat.w = cx * cy * cz + sx * sy * sz;
    quat.x = sx * cy * cz - cx * sy * sz;
    quat.y = cx * sy * cz + sx * cy * sz;
    quat.z = cx * cy * sz - sx * sy * cz;

    return h_core::math::Quaternion::normalize(quat);
}

h_core::math::Vector3 h_core::math::Quaternion::toEuler(
    h_core::math::Quaternion quat) {
    h_core::math::Vector3 eulers;

    // float test = quat.x * quat.y + quat.z * quat.w;
    // ::printf("TEST %f\n", test);
    // if (test > (0.5f - MATH_EPSILON_FLOAT)) {
    //     ::printf("LOCKED max\n");
    //     eulers.x = 2 * ::atan2(quat.x, quat.w);
    //     eulers.y = MATH_PI / 2.f;
    //     eulers.z = 0.f;
    //     return eulers;
    // }
    // if (test < -(0.5f + MATH_EPSILON_FLOAT)) {
    //     ::printf("LOCKED min\n");
    //     eulers.x = -2 * ::atan2(quat.x, quat.w);
    //     eulers.y = -MATH_PI / 2.f;
    //     eulers.z = 0.f;
    //     return eulers;
    // }


    // float sinx_cosy = 2.f * (quat.w * quat.x + quat.y * quat.z);
    // float cosx_cosy = 1.f - 2.f * (quat.x * quat.x + quat.y * quat.y);
    // eulers.x = ::atan2(sinx_cosy, cosx_cosy);

    // float siny = ::sqrt(1.f + 2.f * (quat.w * quat.y - quat.x * quat.z));
    // float cosy = ::sqrt(1.f - 2.f * (quat.w * quat.y - quat.x * quat.z));
    // eulers.y = 2.f * ::atan2(siny, cosy) - (MATH_PI / 2.f);

    // float sinz_cosy = 2.f * (quat.w * quat.z + quat.x * quat.y);
    // float cosz_cosy = 1.f - 2.f * (quat.y * quat.y + quat.z * quat.z);
    // eulers.z = ::atan2(sinz_cosy, cosz_cosy);

    float y = 2.f * (quat.y * quat.z + quat.w * quat.x);
    float x =
        quat.w * quat.w - quat.x * quat.x - quat.y * quat.y + quat.z * quat.z;

    if (::abs(x) < MATH_EPSILON_FLOAT && ::abs(y) < MATH_EPSILON_FLOAT) {
        eulers.x = 2.f * ::atan2(quat.x, quat.w);
    }
    else { eulers.x = ::atan2(y, x); }


    eulers.y = ::asin(
        MATH_CLAMP(-2.f * (quat.x * quat.z - quat.w * quat.y), -1.f, 1.f));

    y = 2.f * (quat.x * quat.y + quat.z * quat.w);
    x = quat.w * quat.w + quat.x * quat.x - quat.y * quat.y - quat.z * quat.z;

    if (abs(x) < MATH_EPSILON_FLOAT && ::abs(y) < MATH_EPSILON_FLOAT) {
        eulers.z = 0.f;
    }
    else { eulers.z = ::atan2(y, x); }


    return eulers;
}
h_core::math::Quaternion h_core::math::Quaternion::rotate(
    h_core::math::Quaternion quat, float angle, h_core::math::Vector3 axis) {
    h_core::math::Vector3 normAxis = axis;

    float length = h_core::math::Vector3::getLength(normAxis);
    if (::abs(length - 1.f) > 0.001f) {  // length can't be < 0
        normAxis = h_core::math::Vector3::normalize(normAxis);
    }

    float halfSin = ::sin(angle * 0.5f);

    return h_core::math::Quaternion::multiply(
        quat,
        h_core::math::Quaternion { normAxis.x * halfSin, normAxis.y * halfSin,
                                   normAxis.z * halfSin, ::cos(angle * 0.5f) });
}

h_core::math::Quaternion h_core::math::Quaternion::multiply(
    h_core::math::Quaternion a, h_core::math::Quaternion b) {
    return h_core::math::Quaternion {
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
        a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
    };
}