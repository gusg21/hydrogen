//
// Created by angus.goucher on 9/8/2024.
//

#pragma once

#include "SDL.h"

#include "curl/curl.h"

#include "core/assets.h"
#include "core/netrequestthreadcontext.h"

#define RUNTIMEASSETS_MAX_RECENT_JOBS 10

namespace h_core {
class RuntimeAssets : public Assets {
  public:
    RuntimeAssets() = default;

    void init(const std::string& serverAddress);
    void destroy();
    void doGUI();

    void loadFromProject(const h_core::project::Project* project);
    void precompile(h_core::RuntimeSystems* systems);
    void flushAndPrecompileNetAssets(h_core::RuntimeSystems* systems);

    template<typename AssetType>
    void requestNetAsset(h_core::AssetIndex index);

    static void netRequestThreadFunction(h_core::NetRequestThreadContext* context);

    bool hasServerConnection();

  private:
    template<typename AssetType>
    static void requestNetAssetNow(
        h_core::Asset** out_assetPtr, CURLcode* out_error, const std::string& serverAddress, h_core::AssetIndex index);

    template<typename AssetType>
    void loadTyped(h_core::project::ProjectAssetEntry assetInfo);

    std::string m_serverAddress {};
    std::thread m_netRequestThread {};
    std::deque<h_core::NetRequestJob> m_recentJobs {};
    h_core::NetRequestThreadContext m_netRequestThreadContext {};
};
}  // namespace h_core

template<typename AssetType>
void h_core::RuntimeAssets::requestNetAsset(h_core::AssetIndex index) {
    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't request non-asset type.");

    NetRequestJob job { m_serverAddress, index, AssetType::getTypeId() };

    m_netRequestThreadContext.jobQueueLock.lock();
    { m_netRequestThreadContext.jobs.push_back(job); }
    m_netRequestThreadContext.jobQueueLock.unlock();

    m_recentJobs.push_back(job);
    if (m_recentJobs.size() > RUNTIMEASSETS_MAX_RECENT_JOBS) m_recentJobs.pop_front();
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
void h_core::RuntimeAssets::requestNetAssetNow(
    h_core::Asset** out_assetPtr, CURLcode* out_error, const std::string& serverAddress, h_core::AssetIndex index) {
    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't request non-asset type.");

    CURL* netHandle = curl_easy_init();
    std::string url = serverAddress + std::string("asset/") + std::to_string(index);
    SDL_Log("INFO: ASSETS: Requesting %s\n...", url.c_str());
    curl_easy_setopt(netHandle, CURLOPT_URL, (void*)url.c_str());
    curl_easy_setopt(netHandle, CURLOPT_WRITEFUNCTION, &netAssetWrite<AssetType>);
    curl_easy_setopt(netHandle, CURLOPT_WRITEDATA, out_assetPtr);
    curl_easy_setopt(netHandle, CURLOPT_CONNECTTIMEOUT, 0.5);
    CURLcode result = curl_easy_perform(netHandle);
    if (result != CURLE_OK) { SDL_Log("WARN: ASSETS: Curl (asset %d) (error code %d): %s\n", index, result, curl_easy_strerror(result)); }
    else { SDL_Log("WARN: ASSETS: Curled asset %d OK\n", index); }
    if (out_error != nullptr) *out_error = result;
}

template<typename AssetType>
void h_core::RuntimeAssets::loadTyped(h_core::project::ProjectAssetEntry assetInfo) {
    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't get asset type that does not derive from Asset");

    if (assetInfo.isRemote) {
        // Load from server
        SDL_Log("WARN: ASSETS: Can't load remote asset #%d without RuntimeAssets implementation\n", assetInfo.index);
        requestNetAsset<AssetType>(assetInfo.index);
    }
    else {
        // Load from file
        AssetType* asset = new AssetType();
        loadAssetFromFile<AssetType>(asset, assetInfo.assetPath);
        m_assets[assetInfo.index] = asset;
    }
}