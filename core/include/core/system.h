#pragma once

#include "core/transform.h"
#include "core/componentbitmask.h"

namespace h_core {
class System {
  public:
    System() = default;

    virtual void process() {};
    virtual h_core::ComponentBitmask getMask() = 0;

    h_core::Transform* transform;
};
}  // namespace h_core
