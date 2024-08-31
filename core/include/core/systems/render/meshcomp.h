//
// Created by angus.goucher on 8/31/2024.
//

#pragma once
#include "core/systems/render/meshasset.h"

namespace h_core {
namespace render {
class MeshComp {
  public:
    void init(h_core::render::MeshAsset* mesh);

    h_core::render::MeshAsset* mesh;
};
}  // namespace render
}  // namespace h_core