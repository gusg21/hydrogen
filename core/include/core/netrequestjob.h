//
// Created by Angus Goucher on 9/6/2024.
//

#pragma once

#include "core/assetindex.h"

namespace h_core {
class NetRequestJob {
  public:
    NetRequestJob() = default;
    NetRequestJob(const std::string& serverAddress, AssetIndex assetIndex, uint32_t assetType)
        : serverAddress(serverAddress), assetIndex(assetIndex), assetType(assetType) {}

    std::string serverAddress {};
    h_core::AssetIndex assetIndex = ASSET_INDEX_BAD;
    uint32_t assetType = UINT32_MAX;
};
}  // namespace h_core