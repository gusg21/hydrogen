//
// Created by xavier.olmstead on 9/7/2024.
//

#pragma once

#include "core/asset.h"
#include "core/math/math.h"

namespace h_core {
namespace physics {
class RigidbodyAsset : public Asset {
  public:
    uint32_t initFromYaml(h_core::Assets* assets, YAML::Node node) override;

    [[nodiscard]] h_core::math::Vector3 getVelocity();
    [[nodiscard]] h_core::math::Vector3 getAcceleration();
    [[nodiscard]] h_core::math::Vector3 getAccumulatedForce();

    [[nodiscard]] float getMass();
    [[nodiscard]] float getInverseMass();

  private:
    h_core::math::Vector3 velocity;
    h_core::math::Vector3 acceleration;
    h_core::math::Vector3 accumulatedForce;

    float mass;
    float inverseMass;
};
}
}