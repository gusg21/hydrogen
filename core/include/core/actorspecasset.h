#pragma once

#include "core/asset.h"
#include "core/componentbitmask.h"
#include "core/systems/script/scriptcomp.h"
#include "core/transform.h"

namespace h_core {
class Assets;

class ActorSpecAsset : public Asset {
  public:
    uint32_t initFromYaml(
        h_core::Assets* assets, const h_core::AssetDescription& desc, const YAML::Node& yaml) override;

    h_core::ComponentBitmask mask;
    h_core::Transform transform;
    h_core::AssetIndex meshIndex;
    h_core::AssetIndex scriptIndex;

    HYASSET(1);
};
}  // namespace h_core