#pragma once

#include "bgfx/bgfx.h"
#include "core/math/vector3.h"
#include "core/math/vector2.h"

#define MODEL_COMPONENT_BITMASK (1 << 1)

namespace h_core {
class Model {
  public:
    Model() = default;

    void init();

  private:
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
};

struct VertexData{
    h_core::math::Vector3 position;
    h_core::math::Vector3 normal;
    h_core::math::Vector2 uv;

    static void init();

    static bgfx::VertexLayout layout;
};

}  // namespace h_core