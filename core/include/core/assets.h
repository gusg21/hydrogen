#pragma once

#include <type_traits>  // grumble grumble

#include <stdint.h>
#include <stdio.h>

#include "bx/bx.h"
#include "tinystl/string.h"
#include "tinystl/unordered_map.h"
#include "tinystl/vector.h"
#include "yaml-cpp/yaml.h"

#include "core/asset.h"

#define ASSET_LOAD_FAIL_CANT_OPEN_FILE 1
#define ASSET_LOAD_FAIL_FILE_TOO_BIG   2

namespace h_core {
typedef uint32_t AssetHash;

// Base class for all sources of assets (packed/unpacked)
class Assets {
  public:
    Assets() = default;

    /// @brief convert the name of an asset to its hash
    /// @param string the string to convert (asset name)
    /// @return the hash
    static AssetHash getAssetHashFromString(tinystl::string string);

    template<typename AssetType>
    uint32_t loadAssetFromFile(tinystl::string filePath);

  private:
    tinystl::vector<h_core::Asset> m_assets {};
    tinystl::unordered_map<h_core::AssetHash, uint32_t>
        m_assetIndexLut {};  // hash -> index
};
}  // namespace h_core

template<typename AssetType>
inline uint32_t h_core::Assets::loadAssetFromFile(tinystl::string filePath) {
    static_assert(
        std::is_base_of<h_core::Asset, AssetType>::value,
        "Can't load asset type that does not derive from Asset");

    // Load file
#define ASSETS_MAX_YAML_LENGTH 4096
    char yamlBuffer[ASSETS_MAX_YAML_LENGTH] = { 0 };
    FILE* yamlFile;
    errno_t result = fopen_s(&yamlFile, filePath.c_str(), "r");
    if (result != 0) {
        printf_s("failed to open asset from file %s\n", filePath.c_str());
        return ASSET_LOAD_FAIL_CANT_OPEN_FILE;
    }
    size_t bytesRead = fread_s(
        yamlBuffer, ASSETS_MAX_YAML_LENGTH, sizeof(char),
        ASSETS_MAX_YAML_LENGTH, yamlFile);
    if (bytesRead >= ASSETS_MAX_YAML_LENGTH) {
        return ASSET_LOAD_FAIL_FILE_TOO_BIG;
    }

    // Parse YAML and load asset
    YAML::Node yaml = YAML::Load(yamlBuffer);
    h_core::Asset asset = AssetType();
    asset.loadFromYaml(yaml);

    return 0;
}