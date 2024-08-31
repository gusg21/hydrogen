#pragma once

#include "core/systems/render/mesh.h"

namespace h_core {
namespace render {
class ModelComp {
  public:
    h_core::render::Mesh
        mesh;  // Just one mesh for now - will be multiple at some point
};
}  // namespace render

}  // namespace h_core
