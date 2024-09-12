//
// Created by xavier.olmstead on 9/7/2024.
//

#pragma once

#include <core/component.h>

#include "core/physics/rigidbodyasset.h"

namespace h_core{
namespace physics{
class RigidbodyComp : public Component {
  public:
    void init(h_core::physics::RigidbodyAsset* rigidbody);

    RigidbodyAsset* rigidbody;
    HYCOMPONENT(3);
};
}
}