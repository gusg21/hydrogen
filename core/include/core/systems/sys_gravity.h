#pragma once

#include "core/system.h"

namespace h_core {
namespace systems {
class Gravity : public System {
  public:
    Gravity() = default;

    void process();
    ComponentBitmask getMask();
};
}  // namespace systems
}  // namespace h_core