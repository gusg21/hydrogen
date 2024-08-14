#pragma once

#include <bx/math.h>

#define TRANSFORM_COMPONENT_BITMASK (1 << 0)

namespace h_core {
class Transform {
  public:
    Transform() : position(0, 0, 0), rotation(0, 0, 0, 1), scale(1, 1, 1) {};

    /// @brief position
    bx::Vec3 position;

    /// @brief rotation (as quaternion)
    bx::Quaternion rotation;

    /// @brief scale
    bx::Vec3 scale;
};
}  // namespace h_core
