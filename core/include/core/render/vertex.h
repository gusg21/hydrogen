#pragma once

#include "core/math/math.h"

namespace h_core {
namespace render {
struct Vertex {
    h_core::math::Vector3 position;
    h_core::math::Vector3 normal;
    h_core::math::Vector2 texCoord;
};
}  // namespace render
}  // namespace h_core
