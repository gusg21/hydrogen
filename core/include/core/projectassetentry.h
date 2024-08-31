//
// Created by angus.goucher on 8/29/2024.
//

#pragma once

#include <stdint.h>
#include <string>

namespace h_core {
struct ProjectAssetEntry {
    ProjectAssetEntry(uint32_t index, uint32_t typeId, std::string assetPath)
        : index(index), typeId(typeId), assetPath(assetPath) {}

    uint32_t index;
    uint32_t typeId;
    std::string assetPath;
};
}