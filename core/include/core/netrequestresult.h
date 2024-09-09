//
// Created by Angus Goucher on 9/7/2024.
//

#pragma once

#include "core/asset.h"
#include "core/assetindex.h"

namespace h_core {
class NetRequestResult {
  public:
    NetRequestResult(h_core::Asset* newAsset, NetRequestJob job, bool success)
        : newAsset(newAsset), job(job), success(success) {}

    NetRequestJob job {std::string {}, ASSET_INDEX_BAD, UINT32_MAX};

    h_core::Asset* newAsset = nullptr;
    bool success = false;
};
}  // namespace h_core
