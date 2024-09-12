//
// Created by angus.goucher on 9/8/2024.
//

#pragma once

#include "SDL.h"

#include "curl/curl.h"

#include "core/assets.h"
#include "core/netrequestthreadcontext.h"
#include "core/runtimeconsole.h"

#define RUNTIMEASSETS_MAX_RECENT_JOBS 10

namespace h_core {
class RuntimeAssets : public Assets {
  public:
    RuntimeAssets() = default;

    void init(const std::string& serverAddress);
    void registerCommands(h_core::RuntimeConsole* console);
    void destroy();
    void doGUI();

    void loadFromProject(const h_core::project::Project* project) override;
    void precompile(h_core::RuntimeSystems* systems);
    void flushAndPrecompileNetAssets(h_core::RuntimeSystems* systems);

    template<typename AssetType>
    void requestNetAsset(h_core::AssetIndex index);

    static void netRequestThreadFunction(h_core::NetRequestThreadContext* context);

    bool hasServerConnection();

    template<typename AssetType>
    void loadAsset(h_core::AssetIndex assetIndex, const std::string& assetFile, bool isRemote);

  private:
    template<typename AssetType>
    static void requestNetAssetNow(
        h_core::Asset** out_assetPtr, CURLcode* out_error, const std::string& serverAddress, h_core::AssetIndex index);

    static uint32_t command_loadAsset(const std::string& arguments, void* data);

    std::string m_serverAddress {};
    std::thread m_netRequestThread {};
    h_core::NetRequestThreadContext m_netRequestThreadContext {};

    std::deque<h_core::NetRequestJob> m_recentJobs {};
    std::unordered_map<uint32_t, uint32_t> m_packedSizeMap {};
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

    HYLOG_DEBUG("ASSETS: Net asset write %d bytes\n", byteCount);

    return byteCount;
}

template<typename AssetType>
void h_core::RuntimeAssets::requestNetAssetNow(
    h_core::Asset** out_assetPtr, CURLcode* out_error, const std::string& serverAddress, h_core::AssetIndex index) {
    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't request non-asset type.");

    std::string url = serverAddress + std::string("asset/") + std::to_string(index);
    HYLOG_INFO("ASSETS: Requesting %s...\n", url.c_str());

    CURL* netHandle = curl_easy_init();
    curl_easy_setopt(netHandle, CURLOPT_URL, (void*)url.c_str());
    curl_easy_setopt(netHandle, CURLOPT_WRITEFUNCTION, &netAssetWrite<AssetType>);
    curl_easy_setopt(netHandle, CURLOPT_WRITEDATA, out_assetPtr);
    curl_easy_setopt(netHandle, CURLOPT_CONNECTTIMEOUT, 0.5);

    CURLcode result = curl_easy_perform(netHandle);

    if (result != CURLE_OK) { HYLOG_WARN("ASSETS: Curl (asset %d) (error code %d): %s\n", index, result, curl_easy_strerror(result)); }
    else { HYLOG_INFO("ASSETS: Curled asset %d OK\n", index); }

    if (out_error != nullptr) *out_error = result;
}

template<typename AssetType>
void h_core::RuntimeAssets::loadAsset(h_core::AssetIndex assetIndex, const std::string& assetFile, bool isRemote) {
    ASSERT_TYPE_IS_ASSET_TYPE(AssetType, "Can't get asset type that does not derive from Asset");

    if (isRemote) {
        // Load from server
        requestNetAsset<AssetType>(assetIndex);
    }
    else {
        // Load from file
        AssetType* asset = new AssetType();
        loadAssetFromFile<AssetType>(asset, assetFile);
        m_assets[assetIndex] = asset;
    }
}