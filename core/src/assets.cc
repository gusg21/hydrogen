#include "core/assets.h"

#include <string>

#include "core/project/project.h"
#include "core/systems/render/meshasset.h"

#define LOAD_ASSET_TYPE_CASE(type)            \
    case type::getTypeId(): {                 \
        loadTyped<type>(m_assets, assetInfo); \
        break;                                \
    }

void h_core::Assets::init(h_core::Systems* systems) {
    m_systems = systems;
}

h_core::AssetHash h_core::Assets::getAssetHashFromString(std::string string) {
    return std::hash<std::string>()(string);
}

void h_core::Assets::precompile(h_core::Systems* systems) {
    for (int assetIndex = 0; assetIndex < ASSETS_MAX_ASSET_COUNT;
         ++assetIndex) {
        h_core::Asset* asset = m_assets[assetIndex];
        if (asset != nullptr) { asset->precompile(systems); }
    }
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