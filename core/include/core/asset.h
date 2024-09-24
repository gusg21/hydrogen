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
    // Asset(const h_core::Asset& asset) = delete;
    Asset& operator=(const h_core::Asset& asset) = delete;

    virtual uint32_t initFromYaml(const YAML::Node& node);
    virtual void saveToYaml(YAML::Node yaml);
    virtual uint32_t precompile(h_core::RuntimeSystems* systems);
    [[nodiscard]] virtual size_t getPackedSize() const;
    virtual void toPacked(uint8_t* _writeHead);
    virtual void fromPacked(const uint8_t* _readHead);
    virtual void doGUI();

    // h_core::AssetDescription desc {};

    HYASSET(UINT32_MAX);
};
}  // namespace h_core