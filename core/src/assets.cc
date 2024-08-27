#include "core/assets.h"

#include <string>

#include "core/project.h"

h_core::AssetHash h_core::Assets::getAssetHashFromString(std::string string) {
    return std::hash<std::string>()(string);
}

void h_core::Assets::loadFromProject(h_core::Project* project) {
    for (h_core::ProjectAssetEntry assetInfo : project->assets) {
        switch (assetInfo.typeId) {
            case 0: { // Scene spec
                h_core::SceneSpec* spec = new h_core::SceneSpec();
                loadAssetFromFile<h_core::SceneSpec>(spec, assetInfo.assetPath);
                m_assets[assetInfo.index] = spec;
                break;
            }
        }
    }
}