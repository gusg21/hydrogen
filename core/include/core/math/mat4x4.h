#pragma once

#include "core/math/quaternion.h"
#include "core/math/vector3.h"

#define MATH_PI 3.14159265358979323846f

namespace h_core {
namespace math {

enum MATH_ROTATION_AXIS {
    X,
    Y,
    Z
};

struct Mat4x4 {
    Mat4x4(float data[16]) {
        for (int i = 0; i < 16; i++) {
            m_matrix[i] = data[i];
        }
    }  // TODO: this is real bad, but I'm tired (felt - gus)

    float m_matrix[16];

    void Translate(Vector3 position);
    void Rotation(float angle, MATH_ROTATION_AXIS axis);
    void Rotation(Quaternion rotation);
    void Scale(Vector3 scale);
    
    static Mat4x4 createDefaultTransformMatrix(); 

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