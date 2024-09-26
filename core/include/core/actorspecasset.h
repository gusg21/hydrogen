#pragma once

#include "core/asset.h"
#include "core/componentbitmask.h"
#include "core/script/scriptcomp.h"
#include "core/transform.h"
#include "core/physics/rigidbody.h"

namespace h_core {
class Assets;

class ActorSpecAsset : public Asset {
  public:
    ActorSpecAsset() = default;

    uint32_t initFromYaml(const std::string& basePath, const YAML::Node& yaml) override;
    void saveToYaml(YAML::Node& yaml) override;

    h_core::ComponentBitmask mask;
    h_core::Transform transform;
    h_core::AssetIndex meshIndex = ASSET_INDEX_BAD;
    h_core::AssetIndex scriptIndex = ASSET_INDEX_BAD;
    h_core::physics::Rigidbody* rigidbody = nullptr;

    HYASSET(1);
};
}  // namespace h_core