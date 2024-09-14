#pragma once

#include "core/asset.h"
#include "core/assets.h"
#include "core/componentbitmask.h"
#include "core/systems/script/scriptcomp.h"
#include "core/transform.h"
#include "core/physics/rigidbody.h"

namespace h_core {
class ActorSpecAsset : public Asset {
  public:
    uint32_t initFromYaml(
        h_core::Assets* assets,
        YAML::Node yaml) override;

    h_core::ComponentBitmask mask;
    h_core::Transform transform;
    h_core::AssetIndex meshIndex;
    h_core::AssetIndex scriptIndex;
    h_core::physics::Rigidbody* rigidbody = nullptr;

    HYASSET(1);
};
}  // namespace h_core