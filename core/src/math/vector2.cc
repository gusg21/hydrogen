#include "core/math/vector2.h"

float h_core::math::Vector2::dot(Vector2* lhs, Vector2* rhs) {
    return lhs->x * rhs->x + lhs->y * rhs->y;
}

float h_core::math::Vector2::dot(Vector2* otherVector){
    return h_core::math::Vector2::dot(this, otherVector);
}