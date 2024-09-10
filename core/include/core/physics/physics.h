//
// Created by xavier.olmstead on 9/7/2024.
//

#pragma once

#include "core/system.h"
#include <vector>

namespace h_core {

namespace math {
class Vector3;
}

namespace physics {
class Physics : public System {
  public:
    uint32_t init(h_core::Engine* engine) override;
    void draw() override;
    void endFrame() override;

    h_core::ComponentBitmask getMask() const override;

    h_core::math::Vector3 calculateGravity();

  private:
    static std::vector<RigidbodyComp*> allRigidbodies;
};
}
}
