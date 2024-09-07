//
// Created by Angus Goucher on 9/6/2024.
//

#pragma once

#include "core/assetindex.h"

namespace h_core {
class NetRequestJob {
  public:
    h_core::AssetIndex assetIndex;
    uint32_t assetType;
    h_core::Assets* assets;
};
}