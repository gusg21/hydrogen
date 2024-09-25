//
// Created by Angus Goucher on 9/13/2024.
//

#pragma once

#include <string>

#include "assetindex.h"

namespace h_core {

enum class AssetRemoteMode {
    LOCAL = 0,
    REMOTE_IMMEDIATE,
    REMOTE_ON_REQUEST,
};

struct AssetDescription {
    AssetDescription() = default;
    AssetDescription(h_core::AssetIndex index, uint32_t type, const std::string& path, AssetRemoteMode remote)
        : index(index), type(type), path(path), remote(remote) {}



    h_core::AssetIndex index = ASSET_INDEX_BAD;
    uint32_t type = UINT32_MAX;
    std::string path {};
    AssetRemoteMode remote = AssetRemoteMode::LOCAL;
};
}  // namespace h_core