#include "core/assets.h"

#include <string>

#include "core/project/project.h"
#include "core/systems/render/meshasset.h"

h_core::AssetHash h_core::Assets::getAssetHashFromString(const std::string& string) {
    return std::hash<std::string>()(string);
}

void h_core::Assets::loadFromProject(const h_core::project::Project* project) {
    for (const h_core::project::ProjectAssetEntry& assetInfo : project->requiredAssets) {
        CALL_TYPED_FUNC_WITH_ASSET_ID(assetInfo.typeId, loadTyped, assetInfo);
    }
}

uint32_t h_core::Assets::getAssetCount() const {
    return m_assetCount;
}
