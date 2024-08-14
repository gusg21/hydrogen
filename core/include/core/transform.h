#pragma once

#include <bx/math.h>

#include "core/component.h"

#define TRANSFORM_COMPONENT_BITMASK (1 << 0)

namespace h_core {
class Transform : public I_Component {
  public:
    /// @brief position
    bx::Vec3 position;

    /// @brief rotation (as quaternion)
    bx::Quaternion rotation;

    /// @brief scale
    bx::Vec3 scale;
};
}  // namespace h_core
