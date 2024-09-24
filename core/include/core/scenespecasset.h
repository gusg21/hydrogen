#pragma once

#include <vector>

#include <yaml-cpp/yaml.h>

#include "core/asset.h"
#include "core/assetindex.h"
#include "core/yamloverrides.h"

#define SCENESPEC_INIT_FAIL_MISSING_ACTOR_SPEC_PATH 1

namespace h_core {
class Assets;

class SceneSpecAsset : public Asset {
  public:
    uint32_t initFromYaml(h_core::Assets* assets, const h_core::AssetDescription& desc, const YAML::Node& yaml) override;

    std::vector<h_core::AssetIndex> actorSpecIndices {};
    YamlOverride yamlOverride;

    HYASSET(0);
};
}  // namespace h_core
