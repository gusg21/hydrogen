#pragma once

#include "core/systems.h"

#include "yaml-cpp/yaml.h"

#define HYASSET(typeId) constexpr static uint32_t getTypeId() { return typeId; }

namespace h_core {
class Assets;

class Asset {
  public:
    Asset() = default;
    Asset(const h_core::Asset& asset) = delete;
    Asset& operator=(const h_core::Asset& asset) = delete;

    virtual uint32_t initFromYaml(h_core::Assets* assets, YAML::Node node);
    virtual uint32_t precompile(h_core::Systems* systems);

    HYASSET(UINT32_MAX);
};
}  // namespace h_core