#pragma once

#include <cstring>

#include "core/math/quaternion.h"
#include "core/math/vector3.h"

#define MATH_PI 3.14159265358979323846f

namespace h_core {
namespace math {

enum Axis { X, Y, Z };

struct Mat4x4 {
    Mat4x4() {
        for (int i = 0; i < 16; i++) {
            matrix[i] = 0.f;
        }
        matrix[0] = 1.0f;
        matrix[5] = 1.0f;
        matrix[10] = 1.0f;
        matrix[15] = 1.0f;
    }

    Mat4x4(float data[16]) {
        for (int i = 0; i < 16; i++) {
            matrix[i] = data[i];
        }
    }

    float matrix[16];

    static Mat4x4 translation(Vector3 position);
    // void rotation(float angle, Axis axis);
    static Mat4x4 rotation(Quaternion rotation);
    static Mat4x4 scale(Vector3 scale);

    static h_core::math::Mat4x4 multiply(
        h_core::math::Mat4x4 a, h_core::math::Mat4x4 b);

    static Mat4x4 createTransformMatrix(
        h_core::math::Vector3 position, h_core::math::Quaternion rotation,
        h_core::math::Vector3 scale);

    static Mat4x4 lookAtMat(
        h_core::math::Vector3 position, h_core::math::Vector3 target);

    static Mat4x4 getProjMatrix(
        float fovY, float aspect, float nearClipPlane, float farClipPlane);
};
}  // namespace math
}  // namespace h_core