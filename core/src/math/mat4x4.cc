#include "core/math/mat4x4.h"

h_core::math::Mat4x4 lookAtMat(
    h_core::math::Vector3 position, h_core::math::Vector3 target,
    bool isRightHanded = true) {
    h_core::math::Vector3 view =
        (isRightHanded ? h_core::math::Vector3::subtract(&position, &target)
                       : h_core::math::Vector3::subtract(&target, &position));

    h_core::math::Vector3 up = h_core::math::Vector3(0);
    h_core::math::Vector3 right = h_core::math::Vector3(0);

    h_core::math::Vector3 uxv = h_core::math::Vector3::cross(&up, &view);

    if (0.0f == h_core::math::Vector3::dot(&uxv, &uxv)) {
        right = h_core::math::Vector3(isRightHanded ? 1.0f : -1.0f, 0.0f, 0.0f);
    } else {
        right = h_core::math::Vector3::normalize(&up);
    }

    up = h_core::math::Vector3::cross(&view, &right);

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

    mat[12] = -h_core::math::Vector3::dot(&right, &position);
    mat[13] = -h_core::math::Vector3::dot(&up, &position);
    mat[14] = -h_core::math::Vector3::dot(&view, &position);
    mat[15] = 1.0f;

    return h_core::math::Mat4x4(mat);
}