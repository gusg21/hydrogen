#pragma once

namespace h_core {
namespace math {
struct Quaternian {
    Quaternian(float value) : x(value), y(value), z(value), w(value) {}
    Quaternian(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    float x;
    float y;
    float z;
    float w;
};
}  // namespace math
}  // namespace h_core