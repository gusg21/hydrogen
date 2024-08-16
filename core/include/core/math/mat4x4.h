#pragma once

#include "core/math/vector3.h"

namespace h_core {
namespace math {
struct Mat4x4 {
    Mat4x4(float data[16]) {
        for(int i = 0; i < 16; i++) {
            matrix[i] = data[i];
        }
    } //TODO: this is real bad, but I'm tired

    float matrix[16];

    static Mat4x4 lookAtMat(
        Vector3 position, Vector3 target, bool isRightHanded = true);
};
}  // namespace math
}  // namespace h_core