#pragma once

#include <cmath>

#include "core/math/mathdefs.h"
#include "core/math/vector3.h"

namespace h_core {
namespace math {
struct Quaternion {
    Quaternion() : x(0.f), y(0.f), z(0.f), w(1.f) {}
    Quaternion(float value) : x(value), y(value), z(value), w(value) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    float x;
    float y;
    float z;
    float w;

    float getLength();

    static h_core::math::Quaternion normalize(h_core::math::Quaternion a);

    static float dot(h_core::math::Quaternion a, h_core::math::Quaternion b);

    static float getLength(h_core::math::Quaternion a);

    static h_core::math::Quaternion fromEuler(h_core::math::Vector3 eulers);

    static h_core::math::Vector3 toEuler(h_core::math::Quaternion quat);

    static h_core::math::Quaternion rotate(h_core::math::Quaternion quat, float angle, h_core::math::Vector3 axis);
    
    static h_core::math::Quaternion multiply(h_core::math::Quaternion a, h_core::math::Quaternion b);
};
}  // namespace math
}  // namespace h_core