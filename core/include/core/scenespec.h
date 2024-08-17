#pragma once

#include <vector>

#include <yaml-cpp/yaml.h>

#include "core/asset.h"
#include "core/actorspec.h"

#define SCENESPEC_INIT_FAIL_MISSING_ACTOR_SPEC_PATH 1

namespace h_core {
class SceneSpec : public Asset {
  public:
    uint32_t initFromYaml(h_core::Assets* assets, YAML::Node yaml);

    std::vector<h_core::AssetIndex> actorSpecIndices {};
};
}  // namespace h_core
