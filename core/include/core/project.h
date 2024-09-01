#pragma once

#include <string>
#include <unordered_map>

#include "core/math/color.h"
#include "core/scenespecasset.h"
#include "core/projectassetentry.h"

namespace h_core {
class Project {
  public:
    Project() = default;

    void loadFromFile(const std::string& yamlPath);

    /// @brief the name of the project
    std::string name = "Unnamed Project";

    uint32_t windowWidth = 1600;
    uint32_t windowHeight = 900;

    h_core::math::Color clearColor {};

    h_core::AssetIndex initialSceneSpec = ASSETS_ASSET_INDEX_BAD;

    std::vector<h_core::ProjectAssetEntry> requiredAssets {};
};
}  // namespace h_core
