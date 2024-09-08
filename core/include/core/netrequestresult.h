//
// Created by Angus Goucher on 9/7/2024.
//

#pragma once

#include "core/asset.h"
#include "core/assetindex.h"

namespace h_core {
class NetRequestResult {
  public:
    NetRequestResult(h_core::Asset* newAsset, h_core::AssetIndex assetIndex)
        : newAsset(newAsset), assetIndex(assetIndex) {}

    h_core::Asset* newAsset;
    h_core::AssetIndex assetIndex;
};
}  // namespace h_core
