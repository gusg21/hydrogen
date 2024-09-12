//
// Created by xavier.olmstead on 9/7/2024.
//

#pragma once

#include <vector>

#include "core/runtimesystem.h"
#include "rigidbodycomp.h"

namespace h_core {

namespace math {
class Vector3;
}

namespace physics {
class Physics : public RuntimeSystem {
  public:
    Physics() = default;

    uint32_t init(h_core::RuntimeEngine* engine) override;
    void draw() override;
    void endFrame() override;

  protected:
    HYSYSTEM(h_core::Transform::getMask() | h_core::physics::RigidbodyComp::getMask())

  private:
    h_core::math::Vector3 calculateGravity();

    static std::vector<RigidbodyComp*> allRigidbodies;
};
}
}
