//
// Created by xavier.olmstead on 9/7/2024.
//

#pragma once

#include <vector>

#include "core/engine.h"
#include "core/runtimeengine.h"
#include "core/runtimesystem.h"
#include "core/transform.h"
#include "core/physics/rigidbodycomp.h"

namespace h_core {
//class RuntimeEngine;

namespace math {
class Vector3;
}

namespace physics {
class Physics : public h_core::RuntimeSystem {
public:
    Physics() = default;

    uint32_t init(h_core::RuntimeEngine* engine) override;
    void draw() override;
    void endFrame() override;

    HYSYSTEM(h_core::Transform::getMask() | h_core::physics::RigidbodyComp::getMask());

private:
     h_core::math::Vector3 calculateGravity();

     std::vector<RigidbodyAsset*> allRigidbodies = {};
};
}
}