//
// Created by xavier.olmstead on 9/7/2024.
//

#pragma once

#include <core/component.h>

#include "core/math/math.h"
#include "yaml-cpp/yaml.h"

namespace h_core{
namespace physics{
class Rigidbody : public Component {
  public:
    Rigidbody() = default;
    Rigidbody(const h_core::math::Vector3 position, const float mass) : position(position), mass(mass) {};

    void initFromYaml(YAML::Node yaml);
    void clearForces();

    // AssetIndex rigidbody = ASSET_INDEX_BAD;
    h_core::math::Vector3 position {};
    h_core::math::Vector3 velocity {};
    h_core::math::Vector3 acceleration {};
    h_core::math::Vector3 accumulatedForce {};

    float mass = 1;
    float inverseMass = 1;

    HYCOMPONENT(3);
};
}
}