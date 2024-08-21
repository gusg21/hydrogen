#pragma once

#include <cstring>

#include "core/math/quaternion.h"
#include "core/math/vector3.h"

#define MATH_PI 3.14159265358979323846f

namespace h_core {
namespace math {

enum Axis {
    X,
    Y,
    Z
};

struct Mat4x4 {
    Mat4x4() {
        ::memset(matrix, 0, 16);
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

    void translate(Vector3 position);
    void rotation(float angle, Axis axis);
    void rotation(Quaternion rotation);
    void scale(Vector3 scale);
    h_core::math::Mat4x4 multiply(h_core::math::Mat4x4 a, h_core::math::Mat4x4 b);

        static Mat4x4 createTransformMatrix(
            Vector3 position, Quaternion rotation, Vector3 scale);

    static Mat4x4 lookAtMat(
        h_core::math::Vector3 position, h_core::math::Vector3 target,
        bool isRightHanded);

    static Mat4x4 getProjMatrix(
        float fov, float aspectRatio, float nearClipPlane, float farClipPlane,
        bool hasHomogenous, bool isRightHanded);
};
}  // namespace math
}  // namespace h_core