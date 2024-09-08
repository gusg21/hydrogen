#include "core/assets.h"

#include <string>
#include <thread>

#include "curl/curl.h"

#include "core/project/project.h"
#include "core/systems/render/meshasset.h"

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

h_core::Assets::Assets() {
    curl_global_init(CURL_GLOBAL_ALL);

    m_netRequestThreadContext.netRequestThreadAlive = false;
    m_netRequestThread = std::thread { netRequestThreadFunction, &m_netRequestThreadContext };
}

h_core::AssetHash h_core::Assets::getAssetHashFromString(const std::string& string) {
    return std::hash<std::string>()(string);
}

void h_core::Assets::precompile(h_core::RuntimeSystems* systems) {
    for (int assetIndex = 0; assetIndex < ASSETS_MAX_ASSET_COUNT; ++assetIndex) {
        h_core::Asset* asset = m_assets[assetIndex];
        if (asset != nullptr) { asset->precompile(systems); }
    }
}

void h_core::Assets::destroy() {
    m_netRequestThreadContext.netRequestThreadAlive = true;
    m_netRequestThread.join();
}

void h_core::Assets::loadFromProject(h_core::project::Project* project) {
    for (const h_core::project::ProjectAssetEntry& assetInfo : project->requiredAssets) {
        //        switch (assetInfo.typeId) {
        //            LOAD_ASSET_TYPE_CASE(h_core::SceneSpecAsset)
        //            LOAD_ASSET_TYPE_CASE(h_core::ActorSpecAsset)
        //            LOAD_ASSET_TYPE_CASE(h_core::script::ScriptAsset)
        //            LOAD_ASSET_TYPE_CASE(h_core::render::MeshAsset)
        //        }
        CALL_TYPED_FUNC_WITH_ASSET_ID(assetInfo.typeId, loadTyped, assetInfo);
    }
}

uint32_t h_core::Assets::getAssetCount() const {
    return m_assetCount;
}

void h_core::Assets::netRequestThreadFunction(h_core::NetRequestThreadContext* context) {
    SDL_Log("INFO: ASSETS: Net request thread running!\n");

    while (!context->netRequestThreadAlive) {
        if (!context->jobs.empty()) {
            // Request asset
            NetRequestJob job = context->jobs.front();
            SDL_Log("INFO: ASSETS: THREAD: got job request for asset %d\n", job.assetIndex);
            h_core::Asset* newAsset;
            CALL_TYPED_FUNC_WITH_ASSET_ID(job.assetType, h_core::Assets::requestNetAssetNow, job.assetIndex, &newAsset);
            SDL_Log("INFO: ASSETS: THREAD: completed net request for asset %d\n", job.assetIndex);

            // Apply to asset list
            SDL_Log("INFO: ASSETS: THREAD: awaiting asset list access... \n", job.assetIndex);
            context->resultQueueLock.lock();
            {
                // Add result to the return list
                context->results.emplace_back(newAsset, job.assetIndex);
            }
            context->resultQueueLock.unlock();
            SDL_Log("INFO: ASSETS: THREAD: updated asset list \n", job.assetIndex);

            // remove job
            context->jobs.pop_front();
        }
    }

    SDL_Log("INFO: ASSETS: Killing net request thread...\n");
}

void h_core::Assets::flushAndPrecompileNetAssets(h_core::RuntimeSystems* systems) {
    m_netRequestThreadContext.resultQueueLock.lock();

    while (!m_netRequestThreadContext.results.empty()) {
        h_core::NetRequestResult result = m_netRequestThreadContext.results.front();

        // Replace old asset
        if (m_assets[result.assetIndex] != nullptr) {
            delete m_assets[result.assetIndex];
        }
        m_assets[result.assetIndex] = result.newAsset;
        result.newAsset->precompile(systems);

        m_netRequestThreadContext.results.pop_front();
    }

    m_netRequestThreadContext.resultQueueLock.unlock();
}
