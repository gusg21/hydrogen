#pragma once

#include "core/system.h"

namespace h_core {
namespace systems {
class Gravity : public System {
  public:
    Gravity() = default;

    uint32_t init(h_core::Engine* engine) override;
    void process() override;
    [[nodiscard]] h_core::ComponentBitmask getMask() const override;
};
}  // namespace systems
}  // namespace h_core