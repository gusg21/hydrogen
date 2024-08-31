#pragma once

#include "core/system.h"

namespace h_core {
namespace gravity {
class Gravity : public System {
  public:
    Gravity() = default;

    uint32_t init();
    void process();
    h_core::ComponentBitmask getMask();
};
}  // namespace systems
}  // namespace h_core