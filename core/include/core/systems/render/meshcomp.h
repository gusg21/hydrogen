//
// Created by angus.goucher on 8/31/2024.
//

#pragma once

#include "core/assetindex.h"
#include "core/component.h"

namespace h_core {
namespace render {
class MeshComp : public Component {
  public:
    void init(h_core::AssetIndex mesh);

    h_core::AssetIndex mesh = ASSET_INDEX_BAD;

    HYCOMPONENT(1);
};
}  // namespace render
}  // namespace h_core