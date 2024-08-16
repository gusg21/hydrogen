#pragma once

namespace h_core {
namespace math {
struct Quaternion {
    Quaternion(float value) : x(value), y(value), z(value), w(value) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    float x;
    float y;
    float z;
    float w;
};
}  // namespace math
}  // namespace h_core