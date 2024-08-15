#pragma once

#include <vector>

#include <yaml-cpp/yaml.h>

#include "core/asset.h"
#include "core/actorspec.h"

namespace h_core {
class SceneSpec : public Asset {
  public:
    void initFromYaml(h_core::Assets* assets, YAML::Node yaml);

    std::vector<h_core::AssetIndex> actorSpecs {};
};
}  // namespace h_core
