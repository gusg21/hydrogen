#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <type_traits>  // grumble grumble
#include <unordered_map>

#include "SDL.h"
#include "yaml-cpp/yaml.h"

#include "core/actorspecasset.h"
#include "core/assetindex.h"
#include "core/log.h"
#include "core/project/projectassetentry.h"
#include "core/scenespecasset.h"
#include "core/systems/render/meshasset.h"
#include "core/systems/script/scriptasset.h"

#define TYPED_SWITCH_CASE(type, func, ...) \
    case type::getTypeId(): {              \
        func<type>(__VA_ARGS__);           \
        break;                             \
    }

#define CALL_TYPED_FUNC_WITH_ASSET_ID(id, func, ...)                         \
    switch ((id)) {                                                          \
        TYPED_SWITCH_CASE(h_core::SceneSpecAsset, func, ##__VA_ARGS__);      \
        TYPED_SWITCH_CASE(h_core::ActorSpecAsset, func, ##__VA_ARGS__);      \
        TYPED_SWITCH_CASE(h_core::script::ScriptAsset, func, ##__VA_ARGS__); \
        TYPED_SWITCH_CASE(h_core::render::MeshAsset, func, ##__VA_ARGS__);   \
    }

#define ASSERT_TYPE_IS_ASSET_TYPE(type, error_msg) static_assert(std::is_base_of_v<h_core::Asset, type>, error_msg)

#define ASSETS_LOAD_FAIL_CANT_OPEN_FILE 1
#define ASSETS_LOAD_FAIL_FILE_TOO_BIG   2

#define ASSETS_MAX_ASSET_COUNT 128

namespace h_core {
typedef uint32_t AssetHash;

namespace project {
class Project;
}

// Base class for all sources of assets (packed/unpacked)
class Assets
{
  public:
    Assets() = default;

    virtual void loadFromProject(const h_core::project::Project* project);

    /// @brief convert the name of an asset to its hash
    /// @param string the string to convert (asset name)
    /// @return the hash
    static AssetHash getAssetHashFromString(const std::string& string);

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
    AssetType* getAssetByIndex(h_core::AssetIndex index) const;

    uint32_t getAssetCount() const;

    virtual void loadAsset(const AssetDescription& desc);

    static uint32_t determineAssetTypeFromExtension(const std::string& extension);

    static const char* getAssetTypeName(uint32_t assetType); 

  protected:
    template<typename AssetType>
    uint32_t loadAssetFromFile(h_core::Asset** out_asset, const h_core::AssetDescription& desc);

    h_core::Asset* m_assets[ASSETS_MAX_ASSET_COUNT] = {};
    std::unordered_map<h_core::AssetHash, h_core::AssetIndex> m_assetIndexMap {};  // hash -> asset index
    h_core::AssetIndex m_nextAssetIndex = 0;
    uint32_t m_assetCount = 0;
};
}  // namespace h_core

template<typename AssetType>
uint32_t h_core::Assets::loadAssetFromFile(h_core::Asset** out_asset, const h_core::AssetDescription& desc) {
    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't load asset type that does not derive from Asset");

    // Load file
    const char* fileText = (const char*)SDL_LoadFile(desc.path.c_str(), nullptr);

    if (fileText == nullptr) {
        HYLOG_DEBUG("ASSETS: Wrong file path %s", desc.path.c_str());
        return 1;  // TODO: Fix
    }

    // Parse YAML and load asset
    YAML::Node yaml = YAML::Load(fileText);

    (*out_asset) = new AssetType();
    (*out_asset)->initFromYaml(this, desc, yaml);

    m_assetCount++;

    return 0;
}

template<typename AssetType>
h_core::AssetIndex h_core::Assets::getOrLoadAsset(std::string filePath) {
    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't load asset type that does not derive from Asset");

    // TODO: This hashing piece to the loading can probably be removed
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
            HYLOG_ERROR("ASSETS: Failed to load asset %s\n", filePath.c_str());
            return ASSET_INDEX_BAD;
        }
        else {
            m_assets[assetIndex] = asset;
            m_assetIndexMap[hash] = assetIndex;
            return assetIndex;
        }
    }

    return ASSET_INDEX_BAD;
}

template<typename AssetType>
AssetType* h_core::Assets::getAssetByIndex(h_core::AssetIndex index) const {
    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't get asset type that does not derive from Asset");

    if (index == ASSET_INDEX_BAD) return nullptr;

    return static_cast<AssetType*>(m_assets[index]);
}