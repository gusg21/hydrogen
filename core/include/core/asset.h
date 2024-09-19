#pragma once

#include "assetdescription.h"
#include "yaml-cpp/yaml.h"

#define HYASSET(typeId)                     \
    constexpr static uint32_t getTypeId() { \
        return typeId;                      \
    }

namespace h_core {
class Assets;
class RuntimeSystems;

class Asset {
  public:
    Asset() = default;
    Asset(const h_core::Asset& asset) = delete;
    Asset& operator=(const h_core::Asset& asset) = delete;

    virtual uint32_t initFromYaml(h_core::Assets* assets, const h_core::AssetDescription& desc, const YAML::Node& node);
    virtual std::vector<uint8_t>* toPacked();
    virtual void fromPacked(const void* packedData, size_t length);

    virtual uint32_t precompile(h_core::RuntimeSystems* systems);
    virtual void doGUI();

    h_core::AssetDescription desc {};

    HYASSET(UINT32_MAX);
};
}  // namespace h_core