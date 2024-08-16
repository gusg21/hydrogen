#pragma once

#include "core/asset.h"
#include "core/assets.h"
#include "core/componentbitmask.h"
#include "core/transform.h"

namespace h_core {
class ActorSpec : public Asset {
  public:
    void initFromYaml(h_core::Assets* assets, YAML::Node yaml);

    h_core::ComponentBitmask mask;
    h_core::Transform transform;
};
}  // namespace h_core