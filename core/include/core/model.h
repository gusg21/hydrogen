#pragma once

#include "bgfx/bgfx.h"

#include "core/asset.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"

#define MODEL_COMPONENT_BITMASK            (1 << 1)
#define MODEL_INIT_FAIL_BAD_GLTF_FILE_PATH 1
#define MODEL_INIT_FAIL_BAD_GLTF           2

namespace h_core {
class Model : public Asset {
  public:
    Model() = default;

    uint32_t initFromYaml(h_core::Assets* assets, YAML::Node node);

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

struct Vertex {
    Vertex() = default;

    h_core::math::Vector3 position;
    h_core::math::Vector3 normal;
    h_core::math::Vector2 uv;

    static void init();

    static bgfx::VertexLayout layout;
};

}  // namespace h_core