#pragma once

#include "core/componentbitmask.h"
#include "core/transform.h"

namespace h_core {
class ActorSpec {
  public:
    h_core::ComponentBitmask mask;
    h_core::Transform transform;
};
}  // namespace h_core