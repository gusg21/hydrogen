#pragma once

#include <string>
#include <unordered_map>

#include "core/math/color.h"
#include "core/scenespecasset.h"
#include "core/project/projectassetentry.h"
#include "core/project/projectactionentry.h"

namespace h_core {
namespace project {
class Project {
  public:
    Project() noexcept = default;

    uint32_t loadFromFile(const std::string& yamlPath, const std::string& assetsBasePath);

    /// @brief the name of the project
    std::string name = "Unnamed Project";

    uint32_t windowWidth = 1600;
    uint32_t windowHeight = 900;

    h_core::math::Color clearColor {};

    h_core::AssetIndex initialSceneSpec = ASSET_INDEX_BAD;

    std::string assetServerAddress {};

    std::vector<h_core::project::ProjectAssetEntry> requiredAssets {};
    std::vector<h_core::project::ProjectActionEntry> actions {};

    // Helpers
    bool hasAssetPath(const std::string& assetPath) const;
    h_core::AssetIndex getOpenIndex() const;
    void removeByPath(const std::string& assetPath);
};
}
}  // namespace h_core
