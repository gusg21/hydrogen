#pragma once

#include "yaml-cpp/yaml.h"

namespace h_core {
class Assets;

class Asset {
  public:
    virtual uint32_t initFromYaml(h_core::Assets* assets, YAML::Node node);
};
}  // namespace h_core