#pragma once

#include "core/asset.h"
#include "core/assets.h"
#include "core/componentbitmask.h"
#include "core/systems/script/scriptcomp.h"
#include "core/transform.h"

namespace h_core {
class ActorSpecAsset : public Asset {
  public:
    uint32_t initFromYaml(
        h_core::Assets* assets, h_core::Systems* systems,
        YAML::Node yaml) override;
    uint32_t initFromFile(
        h_core::Assets* assets, h_core::Systems* systems,
        std::string filepath) override;

    h_core::ComponentBitmask mask;
    h_core::Transform transform;
    h_core::AssetIndex meshIndex;
    h_core::AssetIndex scriptIndex;
};
}  // namespace h_core