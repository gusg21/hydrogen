#pragma once

#include "core/runtimesystem.h"
#include "core/transform.h"

namespace h_core {
namespace systems {
class Gravity : public RuntimeSystem {
  public:
    Gravity() = default;

    uint32_t init(h_core::RuntimeEngine* engine) override;
    void process() override;

    HYSYSTEM(h_core::Transform::getMask());
};
}  // namespace systems
}  // namespace h_core