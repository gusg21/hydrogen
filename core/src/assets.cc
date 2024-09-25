#include "core/assets.h"

#include <string>

#include "core/project/project.h"
#include "core/render/modelasset.h"

h_core::AssetHash h_core::Assets::getAssetHashFromString(const std::string& string) {
    return std::hash<std::string>()(string);
}

void h_core::Assets::loadFromProject(const h_core::project::Project* project) {
    m_project = project;
    for (const h_core::project::ProjectAssetEntry& assetInfo : project->requiredAssets) {
        loadAsset(h_core::AssetDescription {
            assetInfo.index, assetInfo.typeId, assetInfo.assetPath, AssetRemoteMode::LOCAL  // never remote in base assets
        });
    }
}

void h_core::Assets::loadAsset(const AssetDescription& desc) {
    // Load from file

    h_core::Asset* asset = nullptr;
    CALL_TYPED_FUNC_WITH_ASSET_ID(desc.type, loadAssetFromFile, &asset, desc);
    m_assets[desc.index] = static_cast<h_core::Asset*>(asset);
}

uint32_t h_core::Assets::determineAssetTypeFromExtension(const std::string& extension) {
    static const std::unordered_map<std::string, uint32_t> assetTypeLUT = {
        { "hyactor", h_core::ActorSpecAsset::getTypeId() },
        { "hymodel", h_core::render::ModelAsset::getTypeId() },
        { "hyscript", h_core::script::ScriptAsset::getTypeId() },
        { "hyscene", h_core::SceneSpecAsset::getTypeId() }
    };

    return assetTypeLUT.find(extension) == assetTypeLUT.end() ? UINT32_MAX : assetTypeLUT.at(extension);
}

const char* h_core::Assets::getAssetTypeName(uint32_t assetType) {
    static const std::unordered_map<uint32_t, const char*> assetNameLUT = {
        { h_core::ActorSpecAsset::getTypeId(), "Actor Spec" },
        { h_core::render::ModelAsset::getTypeId(), "Mesh" },
        { h_core::script::ScriptAsset::getTypeId(), "Script" },
        { h_core::SceneSpecAsset::getTypeId(), "Scene Spec" }
    };

    return assetNameLUT.find(assetType) == assetNameLUT.end() ? "Unknown (Assets::getAssetTypeName)"
                                                              : assetNameLUT.at(assetType);
}

uint32_t h_core::Assets::getAssetCount() const {
    return m_assetCount;
}
