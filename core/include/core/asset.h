#pragma once

#include "tinystl/string.h"
#include "yaml-cpp/yaml.h"

namespace h_core {
class Assets;

class Asset {
  public:
    void initFromYaml(h_core::Assets* assets, YAML::Node node);
};
}  // namespace h_core