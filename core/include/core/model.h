#pragma once

#include "bgfx/bgfx.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "core/asset.h"

#define MODEL_COMPONENT_BITMASK (1 << 1)

namespace h_core {
class Model : public Asset {
  public:
    Model() = default;

    void initFromYaml(h_core::Assets* assets, YAML::Node yaml);

    bgfx::VertexBufferHandle getVertexBuffer() {
        return m_vertexBuffer;
    }
    
    bgfx::IndexBufferHandle getIndexBuffer() {
        return m_indexBuffer;
    }


  private:
    bgfx::VertexBufferHandle m_vertexBuffer;
    bgfx::IndexBufferHandle m_indexBuffer;
};

struct VertexData {
    h_core::math::Vector3 position;
    h_core::math::Vector3 normal;
    h_core::math::Vector2 uv;

    static void init();

    static bgfx::VertexLayout layout;
};

}  // namespace h_core