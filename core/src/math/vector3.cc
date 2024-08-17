#include "core/math/vector3.h"

#include <cmath>

float h_core::math::Vector3::dot(
    h_core::math::Vector3 lhs, h_core::math::Vector3 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

float h_core::math::Vector3::dot(h_core::math::Vector3 otherVector) {
    return h_core::math::Vector3::dot(Vector3(x, y, z), otherVector);
}

// clang-format off
h_core::math::Vector3 h_core::math::Vector3::cross(
    h_core::math::Vector3 v1, h_core::math::Vector3 v2) {
    return h_core::math::Vector3(
        (v1.y * v2.z) - (v1.z * v2.y), 
        (v1.z * v2.x) - (v1.x * v2.z),
        (v1.x * v2.y) - (v1.y * v2.x));
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

h_core::math::Vector3 h_core::math::Vector3::normalize(
    h_core::math::Vector3 v) {
    float distance = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    return h_core::math::Vector3(
        v.x / distance, v.y / distance, v.z / distance);
}