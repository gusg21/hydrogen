#pragma once

#include <stdint.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <thread>
#include <type_traits>  // grumble grumble
#include <unordered_map>
#include <vector>

#include "SDL.h"
#include "curl/curl.h"
#include "yaml-cpp/yaml.h"

#include "core/asset.h"
#include "core/netrequestthreadcontext.h"
#include "core/project/projectassetentry.h"
#include "core/runtimesystems.h"

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
class Assets {
  public:
    Assets();

    void loadFromProject(h_core::project::Project* project);
    void precompile(h_core::RuntimeSystems* systems);
    void flushAndPrecompileNetAssets(h_core::RuntimeSystems* systems);
    void destroy();

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

    template<typename AssetType>
    void requestNetAsset(h_core::AssetIndex index);

    static void netRequestThreadFunction(h_core::NetRequestThreadContext* context);

  private:
    template<typename AssetType>
    static void requestNetAssetNow(h_core::AssetIndex index, h_core::Asset** out_assetPtr);

    template<typename AssetType>
    h_core::AssetIndex loadAssetFromFile(AssetType* out_asset, std::string filePath);

    template<typename AssetType>
    void loadTyped(h_core::project::ProjectAssetEntry assetInfo);

    h_core::Asset* m_assets[ASSETS_MAX_ASSET_COUNT] = {};
    std::unordered_map<h_core::AssetHash, h_core::AssetIndex> m_assetIndexMap {};  // hash -> asset index
    h_core::AssetIndex m_nextAssetIndex = 0;
    uint32_t m_assetCount = 0;

    std::thread m_netRequestThread;
    h_core::NetRequestThreadContext m_netRequestThreadContext {};
    h_core::RuntimeSystems* m_systems;
};
}  // namespace h_core

template<typename AssetType>
inline uint32_t h_core::Assets::loadAssetFromFile(AssetType* out_asset, std::string filePath) {
    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't load asset type that does not derive from Asset");

    // Load file
    const char* fileText = (const char*)SDL_LoadFile(filePath.c_str(), nullptr);

    // Parse YAML and load asset
    YAML::Node yaml = YAML::Load(fileText);
    out_asset->initFromYaml(this, yaml);

    m_assetCount++;

    return 0;
}

template<typename AssetType>
inline h_core::AssetIndex h_core::Assets::getOrLoadAsset(std::string filePath) {
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
        uint32_t result = loadAssetFromFile<AssetType>(asset, m_systems, filePath);
        if (result != 0) {
            ::SDL_Log("ERROR: Failed to load asset %s\n", filePath.c_str());
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
inline AssetType* h_core::Assets::getAssetByIndex(h_core::AssetIndex index) const {
    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't get asset type that does not derive from Asset");

    if (index == ASSET_INDEX_BAD) return nullptr;

    return static_cast<AssetType*>(m_assets[index]);
}

template<typename AssetType>
void h_core::Assets::loadTyped(h_core::project::ProjectAssetEntry assetInfo) {
    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't get asset type that does not derive from Asset");

    if (assetInfo.isRemote) {
        // Load from server
        requestNetAsset<AssetType>(assetInfo.index);
        return;
    }
    else {
        // Load from file
        AssetType* asset = new AssetType();
        loadAssetFromFile<AssetType>(asset, assetInfo.assetPath);
        m_assets[assetInfo.index] = asset;
    }
}

template<typename AssetType>
size_t netAssetWrite(void* buffer, size_t pieceSize, size_t pieceCount, void* out_asset) {
    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't netload asset type that does not inherit from Asset");

    AssetType* asset = new AssetType();
    size_t byteCount = pieceSize * pieceCount;
    asset->fromPacked(buffer, byteCount);
    *(AssetType**)out_asset = asset;

    SDL_Log("INFO: ASSETS: Net asset write %d bytes\n", byteCount);

    return byteCount;
}

template<typename AssetType>
void h_core::Assets::requestNetAssetNow(h_core::AssetIndex index, h_core::Asset** out_assetPtr) {
    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't request non-asset type.");

    CURL* netHandle = curl_easy_init();
    std::string url = std::string("localhost:5000/asset/") + std::to_string(index);
    SDL_Log("INFO: ASSETS: Requesting %s\n...", url.c_str());
    curl_easy_setopt(netHandle, CURLOPT_URL, (void*)url.c_str());
    curl_easy_setopt(netHandle, CURLOPT_WRITEFUNCTION, &netAssetWrite<AssetType>);
    curl_easy_setopt(netHandle, CURLOPT_WRITEDATA, out_assetPtr);
    CURLcode result = curl_easy_perform(netHandle);
    if (result != CURLE_OK) { SDL_Log("WARN: ASSETS: Curl asset %d error: %s\n", index, curl_easy_strerror(result)); }
    else {
        SDL_Log("WARN: ASSETS: Curled asset %d OK\n", index);
    }
}

template<typename AssetType>
void h_core::Assets::requestNetAsset(h_core::AssetIndex index) {
    //    CURL* netHandle = curl_easy_init();
    //    std::string url = std::string("localhost:5000/asset/") + std::to_string(index);
    //    SDL_Log("INFO: ASSETS: Requesting %s\n...", url.c_str());
    //    curl_easy_setopt(netHandle, CURLOPT_URL, (void*)url.c_str());
    //    curl_easy_setopt(netHandle, CURLOPT_WRITEFUNCTION, &netAssetWrite<AssetType>);
    //    curl_easy_setopt(netHandle, CURLOPT_WRITEDATA, &m_assets[index]);
    //    CURLcode result = curl_easy_perform(netHandle);
    //    if (result != CURLE_OK) { SDL_Log("WARN: ASSETS: Curl asset %d error: %s\n", index,
    //    curl_easy_strerror(result)); } else { SDL_Log("WARN: ASSETS: Curled asset %d OK\n", index); }

    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't request non-asset type.");

    m_netRequestThreadContext.jobQueueLock.lock();
    { m_netRequestThreadContext.jobs.emplace_back(index, AssetType::getTypeId()); }
    m_netRequestThreadContext.jobQueueLock.unlock();
}