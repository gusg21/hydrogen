#include "core/assets.h"

#include <string>
#include <thread>

#include "curl/curl.h"

#include "core/project/project.h"
#include "core/systems/render/meshasset.h"

#define LOAD_ASSET_TYPE_CASE(type)  \
    case type::getTypeId(): {       \
        loadTyped<type>(assetInfo); \
        break;                      \
    }


#define REQUEST_ASSET_TYPE_CASE(type)  \
    case type::getTypeId(): {       \
        loadTyped<type>(assetInfo); \
        break;                      \
    }

h_core::Assets::Assets() {
    curl_global_init(CURL_GLOBAL_ALL);

    m_netRequestThreadContext.shouldNetRequestThreadDie = false;
    m_netRequestThreadContext.assetListLock.lock();
    m_netRequestThread = std::thread { netRequestThreadFunction, &m_netRequestThreadContext };
}

h_core::AssetHash h_core::Assets::getAssetHashFromString(const std::string& string) {
    return std::hash<std::string>()(string);
}

void h_core::Assets::precompile(h_core::Systems* systems) {
    for (int assetIndex = 0; assetIndex < ASSETS_MAX_ASSET_COUNT; ++assetIndex) {
        h_core::Asset* asset = m_assets[assetIndex];
        if (asset != nullptr) { asset->precompile(systems); }
    }
}

void h_core::Assets::destroy() {
    m_netRequestThreadContext.shouldNetRequestThreadDie = true;
    m_netRequestThread.join();
}

void h_core::Assets::loadFromProject(h_core::project::Project* project) {
    for (const h_core::project::ProjectAssetEntry& assetInfo : project->requiredAssets) {
        switch (assetInfo.typeId) {
            LOAD_ASSET_TYPE_CASE(h_core::SceneSpecAsset)
            LOAD_ASSET_TYPE_CASE(h_core::ActorSpecAsset)
            LOAD_ASSET_TYPE_CASE(h_core::script::ScriptAsset)
            LOAD_ASSET_TYPE_CASE(h_core::render::MeshAsset)
        }
    }
}

uint32_t h_core::Assets::getAssetCount() const {
    return m_assetCount;
}

void h_core::Assets::netRequestThreadFunction(h_core::NetRequestThreadContext* context) {
    while (!context->shouldNetRequestThreadDie) {
        SDL_Log("INFO: ASSETS: Net request thread running!\n");

        if (!context->jobs.empty()) {
            // Request asset
            NetRequestJob job = context->jobs.front();
            if (job.assetType == h_core::render::MeshAsset::getTypeId()) {
                
            }
        }

    }

    SDL_Log("INFO: ASSETS: Killing net request thread...\n");
}

void h_core::Assets::flushNetAssets() {
    m_netRequestThreadContext.assetListLock.unlock();

    while (!m_netRequestThreadContext.netAssetsFlushDone) {
        // Wait
    }

    m_netRequestThreadContext.assetListLock.lock();
}
