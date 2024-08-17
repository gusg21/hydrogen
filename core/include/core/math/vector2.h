#pragma once

namespace h_core {
namespace math {
struct Vector2 {
    Vector2() : x(0), y(0) {}
    Vector2(float value) : x(value), y(value) {}
    Vector2(float x, float y) : x(x), y(y) {}

    static float dot(Vector2 lhs, Vector2 rhs);
    float dot(Vector2 otherVector);

    float x;
    float y;
};
}  // namespace math
}  // namespace h_core