//
// Created by Angus Goucher on 9/13/2024.
//

#pragma once

#include <string>

#include "assetindex.h"

namespace h_core {
struct AssetDescription {
    AssetDescription() = default;
    AssetDescription(h_core::AssetIndex index, uint32_t type, const std::string& path, bool remote)
        : index(index), type(type), path(path), remote(remote) {}

    h_core::AssetIndex index = ASSET_INDEX_BAD;
    uint32_t type = UINT32_MAX;
    std::string path {};
    bool remote = false;
};
}