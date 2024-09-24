#pragma once

#include "yaml-cpp/yaml.h"

#include "core/math/math.h"
#include "core/component.h"

namespace h_core {
class Transform : public Component {
  public:
    Transform() : position(0, 0, 0), rotation(0, 0, 0, 1), scale(1, 1, 1) {};

    // Populate data from a YAML node
    void initFromYaml(const YAML::Node& yaml);
    [[nodiscard]] YAML::Node saveToYaml() const;
    [[nodiscard]] h_core::math::Mat4x4 getMatrix() const;

    /// @brief position
    h_core::math::Vector3 position;

    /// @brief rotation (as quaternion)
    h_core::math::Quaternion rotation;

    /// @brief scale
    h_core::math::Vector3 scale;

    HYCOMPONENT(0);
};
}  // namespace h_core
