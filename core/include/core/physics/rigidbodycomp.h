//
// Created by xavier.olmstead on 9/7/2024.
//

#pragma once

#include <core/component.h>

#include "core/assetindex.h"
#include "core/physics/rigidbodyasset.h"

namespace h_core{
namespace physics{
class RigidbodyComp : public Component {
  public:
    void init(h_core::AssetIndex rigidbody);

    AssetIndex rigidbody = ASSET_INDEX_BAD;
    HYCOMPONENT(3);
};
}
}