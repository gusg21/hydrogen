#include "core/math/vector3.h"

#include <cmath>

float h_core::math::Vector3::dot(
    h_core::math::Vector3 lhs, h_core::math::Vector3 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

float h_core::math::Vector3::dot(h_core::math::Vector3 otherVector) {
    return h_core::math::Vector3::dot(Vector3(x, y, z), otherVector);
}

// clang-format off
h_core::math::Vector3 h_core::math::Vector3::cross(
    h_core::math::Vector3 v1, h_core::math::Vector3 v2) {
    return h_core::math::Vector3(
        (v1.y * v2.z) - (v2.y * v1.z), 
        (v1.z * v2.x) - (v2.z * v1.x),
        (v1.x * v2.y) - (v2.x * v1.y));
}  // clang-format on

h_core::math::Vector3 h_core::math::Vector3::cross(h_core::math::Vector3 v) {
    return h_core::math::Vector3::cross(Vector3(x, y, z), v);
}

h_core::math::Vector3 h_core::math::Vector3::subtract(
    h_core::math::Vector3 v1, h_core::math::Vector3 v2) {
    return h_core::math::Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

h_core::math::Vector3 h_core::math::Vector3::add(
    h_core::math::Vector3 v1, h_core::math::Vector3 v2) {
    return h_core::math::Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

h_core::math::Vector3 h_core::math::Vector3::scale(
    h_core::math::Vector3 v, float scalar) {
    return h_core::math::Vector3 { v.x * scalar, v.y * scalar, v.z * scalar };
}

float h_core::math::Vector3::getLength(h_core::math::Vector3 v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

h_core::math::Vector3 h_core::math::Vector3::normalize(
    h_core::math::Vector3 v) {
    float length = h_core::math::Vector3::getLength(v);

    return h_core::math::Vector3(v.x / length, v.y / length, v.z / length);
}