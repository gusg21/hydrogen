#include "core/math/mat4x4.h"

#include <stdint.h>
#include <cmath>

h_core::math::Mat4x4 h_core::math::Mat4x4::lookAtMat(
    h_core::math::Vector3 position, h_core::math::Vector3 target) {
    h_core::math::Vector3 forward =
        h_core::math::Vector3::subtract(position, target);
    forward = h_core::math::Vector3::normalize(forward);

    h_core::math::Vector3 yPosAxis {0.f, 1.f, 0.f};
    h_core::math::Vector3 right = yPosAxis.cross(forward);
    right = h_core::math::Vector3::normalize(right);

    h_core::math::Vector3 up = forward.cross(right);
    up = h_core::math::Vector3::normalize(up);

    // h_core::math::Vector3 up = h_core::math::Vector3(0);
    // h_core::math::Vector3 right = h_core::math::Vector3(0);

    // h_core::math::Vector3 uxv = h_core::math::Vector3::cross(up, view);

    // if (0.0f == h_core::math::Vector3::dot(uxv, uxv)) {
    //     right = h_core::math::Vector3(1.0f, 0.0f, 0.0f);
    // }
    // else { right = h_core::math::Vector3::normalize(up); }

    // up = h_core::math::Vector3::cross(view, right);

    h_core::math::Mat4x4 result {};

    result.matrix[0] = right.x;
    result.matrix[4] = up.x;
    result.matrix[8] = view.x;
    result.matrix[12] = 0.0f;

    result.matrix[1] = right.y;
    result.matrix[5] = up.y;
    result.matrix[9] = view.y;
    result.matrix[13] = 0.0f;

    result.matrix[2] = right.z;
    result.matrix[6] = up.z;
    result.matrix[10] = view.z;
    result.matrix[14] = 0.0f;

    result.matrix[3] = -position.dot(right);
    result.matrix[7] = position.dot(right);
    result.matrix[11] = ;
    result.matrix[15] = 1.0f;

    return result;
}

h_core::math::Mat4x4 h_core::math::Mat4x4::getProjMatrix(
    float fovY, float aspect, float nearClipPlane, float farClipPlane) {
    // https://www.songho.ca/opengl/gl_projectionmatrix.html
    // const float height = 1.0f / std::tan(fov * (MATH_PI / 180.0f));
    // const float width = height * 1.0f / aspectRatio;
    const float DEG2RAD = acos(-1.0f) / 180;
    float tangent = tan((fovY / 2.0f) * DEG2RAD);
    float top = nearClipPlane * tangent;
    float right = top * aspect;

    const float clipDiffernce = farClipPlane - nearClipPlane;
    const float aa = -(farClipPlane + nearClipPlane) / clipDiffernce;
    const float bb = -(2.0f * farClipPlane * nearClipPlane) / clipDiffernce;


    h_core::math::Mat4x4 result {};

    result.matrix[0] = nearClipPlane / right;
    result.matrix[5] = nearClipPlane / top;
    result.matrix[10] = aa;
    result.matrix[11] = -1.0f;
    result.matrix[14] = bb;
    result.matrix[15] = 0.f;

    return result;
}

h_core::math::Mat4x4 h_core::math::Mat4x4::createTransformMatrix(
    h_core::math::Vector3 position, h_core::math::Quaternion rotation,
    h_core::math::Vector3 scale) {
    h_core::math::Mat4x4 matrix {};

    h_core::math::Mat4x4 posMat = h_core::math::Mat4x4::translation(position);
    h_core::math::Mat4x4 rotMat = h_core::math::Mat4x4::rotation(rotation);
    h_core::math::Mat4x4 scaleMat = h_core::math::Mat4x4::scale(scale);

    matrix = Mat4x4::multiply(
        Mat4x4::multiply(Mat4x4::multiply(matrix, posMat), rotMat), scaleMat);

    return matrix;
}

h_core::math::Mat4x4 h_core::math::Mat4x4::translation(
    h_core::math::Vector3 position) {
    h_core::math::Mat4x4 matrix {};
    matrix.matrix[12] = position.x;
    matrix.matrix[13] = position.y;
    matrix.matrix[14] = position.z;
    return matrix;
}

// void h_core::math::Mat4x4::rotation(float angle, h_core::math::Axis axis) {
//     switch (axis) {
//         case X:
//             matrix[5] = std::cos(angle);
//             matrix[6] = -std::sin(angle);
//             matrix[9] = std::sin(angle);
//             matrix[10] = std::cos(angle);
//             break;
//         case Y:
//             matrix[0] = std::cos(angle);
//             matrix[2] = std::sin(angle);
//             matrix[8] = -std::sin(angle);
//             matrix[10] = std::cos(angle);
//             break;
//         case Z:
//             matrix[0] = std::cos(angle);
//             matrix[1] = -std::sin(angle);
//             matrix[4] = std::sin(angle);
//             matrix[5] = std::cos(angle);
//             break;
//         default:
//             break;
//     }
// }

h_core::math::Mat4x4 h_core::math::Mat4x4::rotation(
    h_core::math::Quaternion rotation) {
    // https://www.songho.ca/opengl/gl_quaternion.html
    h_core::math::Mat4x4 result {};
    result.matrix[0] = 1.0f - (2.0f * rotation.y * rotation.y) -
                       (2.0f * rotation.z * rotation.z);
    result.matrix[1] =
        (2.0f * rotation.x * rotation.y) + (2.0f * rotation.w * rotation.z);
    result.matrix[2] =
        (2.0f * rotation.x * rotation.z) - (2.0f * rotation.w * rotation.y);
    result.matrix[3] = 0.f;

    result.matrix[4] =
        (2.0f * rotation.x * rotation.y) - (2.0f * rotation.w * rotation.z);
    result.matrix[5] = 1.0f - (2.0f * rotation.x * rotation.x) -
                       (2.0f * rotation.z * rotation.z);
    result.matrix[6] =
        (2.0f * rotation.y * rotation.z) + (2.0f * rotation.w * rotation.x);
    result.matrix[7] = 0.f;

    result.matrix[8] =
        (2.0f * rotation.x * rotation.z) + (2.0f * rotation.w * rotation.y);
    result.matrix[9] =
        (2.0f * rotation.y * rotation.z) + (2.0f * rotation.w * rotation.x);
    result.matrix[10] = 1.0f - (2.0f * rotation.x * rotation.x) -
                        (2.0f * rotation.y * rotation.y);
    result.matrix[11] = 0.f;

    result.matrix[12] = 0.f;
    result.matrix[13] = 0.f;
    result.matrix[14] = 0.f;
    result.matrix[15] = 1.f;

    // matrix[0] =
    //     1.0f - 2.0f * (rotation.y * rotation.y + rotation.z * rotation.z);
    // matrix[5] =
    //     1.0f - 2.0f * (rotation.x * rotation.x + rotation.z * rotation.z);
    // matrix[10] =
    //     1.0f - 2.0f * (rotation.x * rotation.x + rotation.y * rotation.y);

    // matrix[4] = 2.0f * (rotation.x * rotation.y - rotation.z * rotation.w);
    // matrix[8] = 2.0f * (rotation.x * rotation.z + rotation.y * rotation.w);
    // matrix[1] = 2.0f * (rotation.x * rotation.y + rotation.z * rotation.w);

    // matrix[9] = 2.0f * (rotation.y * rotation.z - rotation.x * rotation.w);
    // matrix[2] = 2.0f * (rotation.x * rotation.z - rotation.y * rotation.w);
    // matrix[6] = 2.0f * (rotation.y * rotation.z + rotation.x * rotation.w);

    return result;
}

h_core::math::Mat4x4 h_core::math::Mat4x4::scale(h_core::math::Vector3 scale) {
    h_core::math::Mat4x4 matrix {};
    matrix.matrix[0] = 1 / scale.x;
    matrix.matrix[5] = 1 / scale.y;
    matrix.matrix[10] = 1 / scale.z;
    return matrix;
}

#define INDEX(x, y) (y + (x * 4))
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