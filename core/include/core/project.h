#pragma once

#include <string>
#include <unordered_map>

#include "core/assets.h"
#include "core/math/color.h"
#include "core/scenespec.h"

namespace h_core {
struct ProjectAssetEntry {
    ProjectAssetEntry(uint32_t index, uint32_t typeId, std::string assetPath)
        : index(index), typeId(typeId), assetPath(assetPath) {}

    uint32_t index;
    uint32_t typeId;
    std::string assetPath;
};

class Project {
  public:
    Project() = default;

    void initFromYaml(std::string yamlPath);

    /// @brief the name of the project
    std::string name = "Unnamed Project";

    uint32_t windowWidth = 1600;
    uint32_t windowHeight = 900;

    h_core::math::Color clearColor {};

    h_core::AssetIndex initialSceneSpec = ASSETS_ASSET_INDEX_BAD;

    std::vector<h_core::ProjectAssetEntry> assets {};
};
}  // namespace h_core
