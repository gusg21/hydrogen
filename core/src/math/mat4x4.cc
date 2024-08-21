#include "core/math/mat4x4.h"

#include <stdint.h>
#include <cmath>

h_core::math::Mat4x4 h_core::math::Mat4x4::lookAtMat(
    h_core::math::Vector3 position, h_core::math::Vector3 target,
    bool isRightHanded) {
    h_core::math::Vector3 view =
        (isRightHanded ? h_core::math::Vector3::subtract(position, target)
                       : h_core::math::Vector3::subtract(target, position));

    h_core::math::Vector3 up = h_core::math::Vector3(0);
    h_core::math::Vector3 right = h_core::math::Vector3(0);

    h_core::math::Vector3 uxv = h_core::math::Vector3::cross(up, view);

    if (0.0f == h_core::math::Vector3::dot(uxv, uxv)) {
        right = h_core::math::Vector3(isRightHanded ? 1.0f : -1.0f, 0.0f, 0.0f);
    }
    else { right = h_core::math::Vector3::normalize(up); }

    up = h_core::math::Vector3::cross(view, right);

    float mat[16];

    mat[0] = right.x;
    mat[1] = up.x;
    mat[2] = view.x;
    mat[3] = 0.0f;

    mat[4] = right.y;
    mat[5] = up.y;
    mat[6] = view.y;
    mat[7] = 0.0f;

    mat[8] = right.z;
    mat[9] = up.z;
    mat[10] = view.z;
    mat[11] = 0.0f;

    mat[12] = -h_core::math::Vector3::dot(right, position);
    mat[13] = -h_core::math::Vector3::dot(up, position);
    mat[14] = -h_core::math::Vector3::dot(view, position);
    mat[15] = 1.0f;

    return h_core::math::Mat4x4(mat);
}

h_core::math::Mat4x4 h_core::math::Mat4x4::getProjMatrix(
    float fov, float aspectRatio, float nearClipPlane, float farClipPlane,
    bool hasHomogenous, bool isRightHanded) {
    const float height = 1.0f / std::tan(fov * (MATH_PI / 180.0f));
    const float width = height * 1.0f / aspectRatio;
    const float clipDiffernce = farClipPlane - nearClipPlane;

    const float aa = hasHomogenous
                         ? (farClipPlane + nearClipPlane) / clipDiffernce
                         : farClipPlane / clipDiffernce;
    const float bb = hasHomogenous
                         ? (2.0f * farClipPlane * nearClipPlane) / clipDiffernce
                         : nearClipPlane * aa;

    float result[16];

    result[0] = width;
    result[5] = height;
    result[10] = isRightHanded ? -aa : aa;
    result[11] = isRightHanded ? -1.0f : 1.0f;
    result[14] = -bb;

    return h_core::math::Mat4x4(result);
}

h_core::math::Mat4x4 h_core::math::Mat4x4::createTransformMatrix(
    h_core::math::Vector3 position, h_core::math::Quaternion rotation,
    h_core::math::Vector3 scale) {
    float data[16];
    h_core::math::Mat4x4 matrix(data);

    matrix.translate(position);
    matrix.rotation(rotation);
    matrix.scale(scale);

    return matrix;
}

void h_core::math::Mat4x4::translate(h_core::math::Vector3 position) {
    matrix[3] = position.x;
    matrix[7] = position.y;
    matrix[11] = position.z;
}

void h_core::math::Mat4x4::rotation(float angle, h_core::math::Axis axis) {
    switch (axis) {
        case X:
            matrix[5] = std::cos(angle);
            matrix[6] = -std::sin(angle);
            matrix[9] = std::sin(angle);
            matrix[10] = std::cos(angle);
            break;
        case Y:
            matrix[0] = std::cos(angle);
            matrix[2] = std::sin(angle);
            matrix[8] = -std::sin(angle);
            matrix[10] = std::cos(angle);
            break;
        case Z:
            matrix[0] = std::cos(angle);
            matrix[1] = -std::sin(angle);
            matrix[4] = std::sin(angle);
            matrix[5] = std::cos(angle);
            break;
        default:
            break;
    }
}

void h_core::math::Mat4x4::rotation(h_core::math::Quaternion rotation) {
    matrix[0] =
        1.0f - 2.0f * (rotation.y * rotation.y + rotation.z * rotation.z);
    matrix[5] =
        1.0f - 2.0f * (rotation.x * rotation.x + rotation.z * rotation.z);
    matrix[10] =
        1.0f - 2.0f * (rotation.x * rotation.x + rotation.y * rotation.y);

    matrix[1] = 2.0f * (rotation.x * rotation.y - rotation.z * rotation.w);
    matrix[2] = 2.0f * (rotation.x * rotation.z + rotation.y * rotation.w);
    matrix[4] = 2.0f * (rotation.x * rotation.y + rotation.z * rotation.w);

    matrix[6] = 2.0f * (rotation.y * rotation.z - rotation.x * rotation.w);
    matrix[8] = 2.0f * (rotation.x * rotation.z - rotation.y * rotation.w);
    matrix[9] = 2.0f * (rotation.y * rotation.z + rotation.x * rotation.w);
}

void h_core::math::Mat4x4::scale(h_core::math::Vector3 scale) {
    matrix[0] = 1 / scale.x;
    matrix[5] = 1 / scale.y;
    matrix[10] = 1 / scale.z;
}

#define INDEX(x,y) (y + (x * 4))
h_core::math::Mat4x4 h_core::math::Mat4x4::multiply(
    h_core::math::Mat4x4 a, h_core::math::Mat4x4 b) {
    h_core::math::Mat4x4 dest {};
    for (uint32_t destX = 0; destX < 4; destX++) {
        for (uint32_t destY = 0; destY < 4; destY++) {
            dest.matrix[INDEX(destX, destY)] =
                a.matrix[INDEX(0, destY)] * b.matrix[INDEX(destX, 0)] +
                a.matrix[INDEX(1, destY)] * b.matrix[INDEX(destX, 1)] +
                a.matrix[INDEX(2, destY)] * b.matrix[INDEX(destX, 2)] +
                a.matrix[INDEX(3, destY)] * b.matrix[INDEX(destX, 3)];
        }
    }
    return dest;
}