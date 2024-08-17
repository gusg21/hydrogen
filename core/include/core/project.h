#pragma once

#include <string>

#include "core/assets.h"
#include "core/scenespec.h"

namespace h_core {
class Project {
  public:
    Project() = default;

    /// @brief the name of the project
    std::string projectName = "Unnamed Project";

    /// @brief the assets contained in the project
    h_core::Assets assets = {};

    /// @brief a pointer to the scene spec to initialize on game start
    h_core::AssetIndex initialSceneSpec = ASSETS_ASSET_INDEX_BAD;
};
}  // namespace h_core
