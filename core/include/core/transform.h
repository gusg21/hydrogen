#pragma once

#include "yaml-cpp/yaml.h"

#include "core/math/math.h"

#define TRANSFORM_COMPONENT_BITMASK (1 << 0)

namespace h_core {
class Transform {
  public:
    Transform() : position(0, 0, 0), rotation(0, 0, 0, 1), scale(1, 1, 1) {};

    // Populate data from a YAML node
    void initFromYaml(YAML::Node yaml);
    [[nodiscard]] h_core::math::Mat4x4 getMatrix() const;

    /// @brief position
    h_core::math::Vector3 position;

    /// @brief rotation (as quaternion)
    h_core::math::Quaternion rotation;

    /// @brief scale
    h_core::math::Vector3 scale;
};
}  // namespace h_core
