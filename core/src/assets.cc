#include "core/assets.h"

#include <string>

#include "core/project.h"
#include "core/systems/render/meshasset.h"

#define LOAD_ASSET_TYPE_FROM_NUMBER(type, id)          \
    case id: {                                         \
        loadTyped<type>(m_assets, assetInfo, systems); \
        break;                                         \
    }

void h_core::Assets::init(h_core::Systems* systems) {
    m_systems = systems;
}

h_core::AssetHash h_core::Assets::getAssetHashFromString(std::string string) {
    return std::hash<std::string>()(string);
}

void h_core::Assets::loadFromProject(
    h_core::Project* project, h_core::Systems* systems) {
    for (const h_core::ProjectAssetEntry& assetInfo : project->requiredAssets) {
        switch (assetInfo.typeId) {
            LOAD_ASSET_TYPE_FROM_NUMBER(h_core::SceneSpecAsset, 0)
            LOAD_ASSET_TYPE_FROM_NUMBER(h_core::ActorSpecAsset, 1)
            LOAD_ASSET_TYPE_FROM_NUMBER(h_core::script::ScriptAsset, 2)
            LOAD_ASSET_TYPE_FROM_NUMBER(h_core::render::MeshAsset, 3)
        }
    }
}