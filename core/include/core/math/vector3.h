#pragma once

namespace h_core {
namespace math {
struct Vector3 {
    Vector3() : x(0), y(0), z(0) {}
    explicit Vector3(float value) : x(value), y(value), z(value) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    constexpr Vector3& operator+=(const Vector3& other) {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    constexpr Vector3& operator*=(float other) {
        this->x *= other;
        this->y *= other;
        this->z *= other;
        return *this;
    }

    static float dot(Vector3 lhs, Vector3 rhs);
    [[nodiscard]] float dot(Vector3 otherVector) const;

    static Vector3 cross(Vector3 v1, Vector3 v2);
    [[nodiscard]] Vector3 cross(Vector3 v) const;

    static Vector3 subtract(Vector3 v1, Vector3 v2);
    static Vector3 add(Vector3 v1, Vector3 v2);
    static Vector3 scale(Vector3 v, float scalar);

    static float getLength(Vector3 v);
    static Vector3 normalize(Vector3 v);

    float x;
    float y;
    float z;
};
}  // namespace math
}  // namespace h_core

inline h_core::math::Vector3 operator+(const h_core::math::Vector3& a, const h_core::math::Vector3& b) {
    h_core::math::Vector3 result = a;
    result += b;
    return result;
}

inline h_core::math::Vector3 operator*(const h_core::math::Vector3& a, float b) {
    h_core::math::Vector3 result = a;
    result *= b;
    return result;
}