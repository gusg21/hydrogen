#pragma once

namespace h_core {
namespace math {
struct Vector3 {
    Vector3(float value) : x(value), y(value), z(value) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z){}

    static float dot(Vector3* lhs, Vector3* rhs);
    float dot(Vector3* otherVector);

    static Vector3 cross(Vector3* v1, Vector3* v2);
    Vector3 cross(Vector3* v);

    static Vector3 subtract(Vector3* v1, Vector3* v2);

    static Vector3 normalize(Vector3* v);

    float x;
    float y;
    float z;
};
}  // namespace math
}  // namespace h_core