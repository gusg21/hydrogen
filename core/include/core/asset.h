#pragma once

#include "core/systems.h"

#include "yaml-cpp/yaml.h"

namespace h_core {
class Assets;

class Asset {
  public:
    Asset() = default;
    Asset(const h_core::Asset& asset) = delete;
    Asset& operator=(const h_core::Asset& asset) = delete;

    virtual uint32_t initFromYaml(h_core::Assets* assets, h_core::Systems* systems, YAML::Node node);
};
}  // namespace h_core