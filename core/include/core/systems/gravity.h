#pragma once

#include "core/system.h"

namespace h_core {
namespace systems {
class Gravity : public System {
  public:
    Gravity() = default;

    uint32_t init();
    void process();
    h_core::ComponentBitmask getMask();
};
}  // namespace systems
}  // namespace h_core