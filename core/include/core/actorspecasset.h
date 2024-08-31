#pragma once

#include "core/asset.h"
#include "core/assets.h"
#include "core/componentbitmask.h"
#include "core/transform.h"
#include "core/systems/script/scriptcomp.h"

namespace h_core {
class ActorSpecAsset : public Asset {
  public:
    uint32_t initFromYaml(h_core::Assets* assets, h_core::Systems* systems, YAML::Node yaml);

    h_core::ComponentBitmask mask;
    h_core::Transform transform;
    h_core::AssetIndex meshIndex;
    h_core::AssetIndex scriptIndex;
};
}  // namespace h_core