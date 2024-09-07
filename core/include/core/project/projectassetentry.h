//
// Created by angus.goucher on 8/29/2024.
//

#pragma once

#include <stdint.h>
#include <string>

namespace h_core {
namespace project {
struct ProjectAssetEntry {
    ProjectAssetEntry(
        uint32_t index, uint32_t typeId, const std::string& assetPath, bool isRemote)
        : index(index), typeId(typeId), assetPath(assetPath), isRemote(isRemote) {}

    uint32_t index;
    uint32_t typeId;
    std::string assetPath;
    bool isRemote;
};
}
}