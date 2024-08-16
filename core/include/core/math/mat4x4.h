#pragma once

#include "core/math/vector3.h"

namespace h_core {
namespace math {
struct Mat4x4 {
    Mat4x4(float data[16]) {
        for(int i = 0; i < 16; i++) {
            matrix[i] = data[i];
        }
    } //TODO: this is real bad, but I'm tired (felt - gus)

    float matrix[16];

    static h_core::math::Mat4x4 lookAtMat(
        h_core::math::Vector3 position, h_core::math::Vector3 target,
        bool isRightHanded);
};
}  // namespace math
}  // namespace h_core