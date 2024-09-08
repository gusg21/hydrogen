#pragma once

#include "core/system.h"
#include "core/transform.h"

namespace h_core {
namespace systems {
class Gravity : public System {
  public:
    Gravity() = default;

    uint32_t init(h_core::Engine* engine) override;
    void process() override;

    HYSYSTEM(h_core::Transform::getMask());
};
}  // namespace systems
}  // namespace h_core