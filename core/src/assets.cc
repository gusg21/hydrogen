#include "core/assets.h"

#include <string>

#include "core/project/project.h"
#include "core/systems/render/meshasset.h"

h_core::AssetHash h_core::Assets::getAssetHashFromString(const std::string& string) {
    return std::hash<std::string>()(string);
}

void h_core::Assets::loadFromProject(const h_core::project::Project* project) {
    for (const h_core::project::ProjectAssetEntry& assetInfo : project->requiredAssets) {
        loadAsset(h_core::AssetDescription {
            assetInfo.index, assetInfo.typeId, assetInfo.assetPath, false  // never remote in base assets
        });
    }
}

void h_core::Assets::loadAsset(const AssetDescription& desc) {
    // Load from file
    h_core::Asset* asset = nullptr;
    CALL_TYPED_FUNC_WITH_ASSET_ID(desc.type, loadAssetFromFile, &asset, desc);
    m_assets[desc.index] = static_cast<h_core::Asset*>(asset);
}

uint32_t h_core::Assets::getAssetCount() const {
    return m_assetCount;
}
