#include "core/assets.h"

#include <string>

#include "core/project.h"

void h_core::Assets::init(h_core::Systems* systems) {
    m_systems = systems;
}

h_core::AssetHash h_core::Assets::getAssetHashFromString(std::string string) {
    return std::hash<std::string>()(string);
}

void h_core::Assets::loadFromProject(
    h_core::Project* project, h_core::Systems* systems) {
    for (h_core::ProjectAssetEntry assetInfo : project->assets) {
        switch (assetInfo.typeId) {
            case 0: {  // Scene spec
                loadTyped<h_core::SceneSpecAsset>(m_assets, assetInfo, systems);
                break;
            }
            case 1: {
                loadTyped<h_core::ActorSpecAsset>(m_assets, assetInfo, systems);
                break;
            }
            case 2: {
                loadTyped<h_core::script::ScriptAsset>(m_assets, assetInfo, systems);
                break;
            }
        }
    }
}