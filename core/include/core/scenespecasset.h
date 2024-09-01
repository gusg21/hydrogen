#pragma once

#include <vector>

#include <yaml-cpp/yaml.h>

#include "core/assets.h"
#include "core/actorspecasset.h"

#define SCENESPEC_INIT_FAIL_MISSING_ACTOR_SPEC_PATH 1

namespace h_core {
class SceneSpecAsset : public Asset {
  public:
    uint32_t initFromYaml(h_core::Assets* assets, YAML::Node yaml) override;

    std::vector<h_core::AssetIndex> actorSpecIndices {};

    HYASSET(0);
};
}  // namespace h_core
