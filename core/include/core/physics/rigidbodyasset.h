//
// Created by xavier.olmstead on 9/7/2024.
//

#pragma once

#include <core/actor.h>

#include "core/asset.h"
#include "core/math/math.h"

#define RIGIDBODY_COMPONENT_BITMASK (1 << 3)

namespace h_core {
namespace physics {
class RigidbodyAsset : public Asset {
  public:
    RigidbodyAsset() = default;
    uint32_t initFromYaml(h_core::Assets* assets, YAML::Node node) override;
    void clearForces();

    HYASSET(4);

    h_core::math::Vector3 position {};
    h_core::math::Vector3 velocity {};
    h_core::math::Vector3 acceleration {};
    h_core::math::Vector3 accumulatedForce {};

    float mass = 1;
    float inverseMass = 1;

    ActorId actorId {};
};
}
}