//
// Created by angus.goucher on 9/8/2024.
//

#include "core/runtimeassets.h"

#include "imgui.h"

#include "core/theming/guicolors.h"
#include "core/project/project.h"

void h_core::RuntimeAssets::init(const std::string& serverAddress) {
    curl_global_init(CURL_GLOBAL_ALL);

    m_serverAddress = serverAddress;

    m_netRequestThreadContext.pingServerAddress = serverAddress;
    m_netRequestThreadContext.netRequestThreadAlive = false;
    m_netRequestThread = std::thread { netRequestThreadFunction, &m_netRequestThreadContext };
}

void h_core::RuntimeAssets::destroy() {
    m_netRequestThreadContext.netRequestThreadAlive = true;
    m_netRequestThread.join();
}

void h_core::RuntimeAssets::doGUI() {
    if (ImGui::Begin("Runtime Assets Debugger")) {
        ImGui::TextColored(hasServerConnection() ? IMGUI_COLOR_GOOD : IMGUI_COLOR_WARN, hasServerConnection() ? "CONNECTED TO SERVER" : "NOT CONNECTED TO SERVER");
    }
    ImGui::End();
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

        if (result.success) {
            // Replace old asset
            if (m_assets[result.job.assetIndex] != nullptr) { delete m_assets[result.job.assetIndex]; }
            m_assets[result.job.assetIndex] = result.newAsset;
            result.newAsset->precompile(systems);
        }
        else {
            // Re-queue new asset request
            CALL_TYPED_FUNC_WITH_ASSET_ID(result.job.assetType, RuntimeAssets::requestNetAsset, result.job.assetIndex);
        }

        m_netRequestThreadContext.results.pop_front();
    }

    m_netRequestThreadContext.resultQueueLock.unlock();
}

void h_core::RuntimeAssets::netRequestThreadFunction(h_core::NetRequestThreadContext* context) {
    SDL_Log("INFO: ASSETS: Net request thread running!\n");

    while (!context->netRequestThreadAlive) {
        // Do jobs if needed
        if (!context->jobs.empty()) {
            // Request asset
            NetRequestJob job = context->jobs.front();
            SDL_Log("INFO: ASSETS: THREAD: got job request for asset %d\n", job.assetIndex);
            h_core::Asset* newAsset = nullptr;
            CURLcode result;
            CALL_TYPED_FUNC_WITH_ASSET_ID(
                job.assetType, h_core::RuntimeAssets::requestNetAssetNow, &newAsset, &result, job.serverAddress, job.assetIndex);
            SDL_Log("INFO: ASSETS: THREAD: completed net request for asset %d\n", job.assetIndex);

            // Apply to asset list
            SDL_Log("INFO: ASSETS: THREAD: awaiting asset list access... \n", job.assetIndex);
            context->resultQueueLock.lock();
            {
                // Add result to the return list
                if (newAsset == nullptr) { context->results.emplace_back(nullptr, job, false); }
                else { context->results.emplace_back(newAsset, job, true); }
            }
            context->resultQueueLock.unlock();
            SDL_Log("INFO: ASSETS: THREAD: updated asset list \n", job.assetIndex);

            // remove job
            context->jobs.pop_front();
        }

        // Update connection test
        CURL* connect_test = curl_easy_init();
        curl_easy_setopt(connect_test, CURLOPT_URL, context->pingServerAddress.c_str());
        curl_easy_setopt(connect_test, CURLOPT_CONNECT_ONLY, 1L);
        CURLcode result = curl_easy_perform(connect_test);
        if (result != CURLE_OK && result != CURLE_COULDNT_CONNECT) {
            SDL_Log("WARN: ASSETS: Ping attempt returned curl error code %d\n", result);
        }
        context->hasServerConnection = result == CURLE_OK;
    }

    SDL_Log("INFO: ASSETS: Killing net request thread...\n");
}

bool h_core::RuntimeAssets::hasServerConnection() {
    return m_netRequestThreadContext.hasServerConnection;
}