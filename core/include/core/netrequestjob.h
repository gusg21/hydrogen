//
// Created by Angus Goucher on 9/6/2024.
//

#pragma once

#include "core/assetindex.h"

namespace h_core {
class NetRequestJob {
  public:
    NetRequestJob(AssetIndex assetIndex, uint32_t assetType) : assetIndex(assetIndex), assetType(assetType) {}

    h_core::AssetIndex assetIndex;
    uint32_t assetType;
};
}