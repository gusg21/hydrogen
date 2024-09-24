//
// Created by xavier.olmstead on 9/23/2024.
//

#pragma once
#include "core/math/math.h"

namespace  h_core {
namespace physics {
struct RayHit {
    h_core::math::Vector3 position;

};

class Raycast {
  public:
    RayHit screenToWorld(h_core::math::Vector2 origin, h_core::math::Vector3 direction, float length);
  private:

};
}
}
