#pragma once

#include <type_traits>  // grumble grumble

#include <stdint.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "yaml-cpp/yaml.h"

#include "core/asset.h"

#define ASSETS_LOAD_FAIL_CANT_OPEN_FILE 1
#define ASSETS_LOAD_FAIL_FILE_TOO_BIG   2

#define ASSETS_MAX_ASSET_COUNT 128
#define ASSETS_ASSET_INDEX_BAD UINT32_MAX

namespace h_core {
typedef uint32_t AssetHash;
typedef uint32_t AssetIndex;

class Project;

// Base class for all sources of assets (packed/unpacked)
class Assets {
  public:
    Assets() = default;

    void loadFromProject(h_core::Project* project);

    /// @brief convert the name of an asset to its hash
    /// @param string the string to convert (asset name)
    /// @return the hash
    static AssetHash getAssetHashFromString(std::string string);

    /// @brief loads or retrieves an asset index by its name.
    /// @tparam AssetType the asset type to load (base class h_core::Asset)
    /// @param filePath the path to load
    /// @return the asset index (permanent)
    template<typename AssetType>
    h_core::AssetIndex getOrLoadAsset(std::string filePath);

    /// @brief Get an asset by its index. Note that this pointer is only valid
    /// until the assets are updated again; hold on to an AssetIndex instead.
    /// @tparam AssetType the asset type to load (base class h_core::Asset)
    /// @param index the index to load
    /// @return a pointer (not managed by you!) to the asset
    template<typename AssetType>
    AssetType* getAssetByIndex(h_core::AssetIndex index);

  private:
    template<typename AssetType>
    h_core::AssetIndex loadAssetFromFile(
        AssetType* out_asset, std::string filePath);

    h_core::Asset* m_assets[ASSETS_MAX_ASSET_COUNT] = {};
    std::unordered_map<h_core::AssetHash, h_core::AssetIndex>
        m_assetIndexMap {};  // hash -> asset index
    h_core::AssetIndex m_nextAssetIndex = 0;
};
}  // namespace h_core

template<typename AssetType>
inline uint32_t h_core::Assets::loadAssetFromFile(
    AssetType* out_asset, std::string filePath) {
    static_assert(
        std::is_base_of<h_core::Asset, AssetType>::value,
        "Can't load asset type that does not derive from Asset");

    // Load file
    std::stringstream yamlBufferStream;
    std::ifstream yamlFileStream { filePath };
    yamlBufferStream << yamlFileStream.rdbuf();

    // Parse YAML and load asset
    YAML::Node yaml = YAML::Load(yamlBufferStream.str());
    out_asset->initFromYaml(this, yaml);

    return 0;
}

template<typename AssetType>
inline h_core::AssetIndex h_core::Assets::getOrLoadAsset(std::string filePath) {
    static_assert(
        std::is_base_of<h_core::Asset, AssetType>::value,
        "Can't load asset type that does not derive from Asset");

    h_core::AssetHash hash = getAssetHashFromString(filePath);
    if (m_assetIndexMap.count(hash) > 0) {
        // Load existing asset
        h_core::AssetIndex assetIndex = m_assetIndexMap[hash];
        return assetIndex;
    }
    else {
        // Load new asset
        h_core::AssetIndex assetIndex = m_nextAssetIndex++;
        AssetType* asset = new AssetType();
        uint32_t result = loadAssetFromFile<AssetType>(asset, filePath);
        if (result != 0) {
            printf("ERROR: Failed to load asset %s\n", filePath.c_str());
            return ASSETS_ASSET_INDEX_BAD;
        }
        else {
            m_assets[assetIndex] = asset;
            m_assetIndexMap[hash] = assetIndex;
            return assetIndex;
        }
    }

    return ASSETS_ASSET_INDEX_BAD;
}

template<typename AssetType>
inline AssetType* h_core::Assets::getAssetByIndex(h_core::AssetIndex index) {
    static_assert(
        std::is_base_of<h_core::Asset, AssetType>::value,
        "Can't get asset type that does not derive from Asset");

    return static_cast<AssetType*>(m_assets[index]);
}
