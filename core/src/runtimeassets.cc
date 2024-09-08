//
// Created by angus.goucher on 9/8/2024.
//

#include "core/runtimeassets.h"
#include "core/project/project.h"

void h_core::RuntimeAssets::init() {
    curl_global_init(CURL_GLOBAL_ALL);

    m_netRequestThreadContext.netRequestThreadAlive = false;
    m_netRequestThread = std::thread { netRequestThreadFunction, &m_netRequestThreadContext };
}

void h_core::RuntimeAssets::destroy() {
    m_netRequestThreadContext.netRequestThreadAlive = true;
    m_netRequestThread.join();
}

void h_core::RuntimeAssets::loadFromProject(const h_core::project::Project* project) {
    for (const h_core::project::ProjectAssetEntry& assetInfo : project->requiredAssets) {
        CALL_TYPED_FUNC_WITH_ASSET_ID(assetInfo.typeId, RuntimeAssets::loadTyped, assetInfo);
    }
}

void h_core::RuntimeAssets::precompile(h_core::RuntimeSystems* systems) {
    for (int assetIndex = 0; assetIndex < ASSETS_MAX_ASSET_COUNT; ++assetIndex) {
        h_core::Asset* asset = m_assets[assetIndex];
        if (asset != nullptr) { asset->precompile(systems); }
    }
}

void h_core::RuntimeAssets::flushAndPrecompileNetAssets(h_core::RuntimeSystems* systems) {
    m_netRequestThreadContext.resultQueueLock.lock();

    while (!m_netRequestThreadContext.results.empty()) {
        h_core::NetRequestResult result = m_netRequestThreadContext.results.front();

        // Replace old asset
        if (m_assets[result.assetIndex] != nullptr) { delete m_assets[result.assetIndex]; }
        m_assets[result.assetIndex] = result.newAsset;
        result.newAsset->precompile(systems);

        m_netRequestThreadContext.results.pop_front();
    }

    m_netRequestThreadContext.resultQueueLock.unlock();
}
void h_core::RuntimeAssets::netRequestThreadFunction(h_core::NetRequestThreadContext* context) {
    SDL_Log("INFO: ASSETS: Net request thread running!\n");

    while (!context->netRequestThreadAlive) {
        if (!context->jobs.empty()) {
            // Request asset
            NetRequestJob job = context->jobs.front();
            SDL_Log("INFO: ASSETS: THREAD: got job request for asset %d\n", job.assetIndex);
            h_core::Asset* newAsset;
            CALL_TYPED_FUNC_WITH_ASSET_ID(job.assetType, h_core::RuntimeAssets::requestNetAssetNow, job.assetIndex, &newAsset);
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