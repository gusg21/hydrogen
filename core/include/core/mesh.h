#pragma once

#include "core/asset.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "glad/glad.h"

#define MODEL_COMPONENT_BITMASK            (1 << 1)
#define MODEL_INIT_FAIL_BAD_GLTF_FILE_PATH 1
#define MODEL_INIT_FAIL_BAD_GLTF           2

namespace h_core {
struct Vertex {
    Vertex() = default;

    h_core::math::Vector3 position;
    h_core::math::Vector3 normal;
    h_core::math::Vector2 texCoord;
    
    // static bgfx::VertexLayout layout;
};

struct MeshData {
    std::vector<h_core::Vertex> vertices;
    std::vector<uint32_t> indicies;
};

class Mesh : public Asset {
  public:
    Mesh() = default;

    uint32_t initFromYaml(h_core::Assets* assets, YAML::Node node);
    void loadModel(
        uint32_t vertexCount, const Vertex* vertexBuffer,
        uint32_t inidicesCount, const uint32_t* indexBuffer);
    //   bgfx::VertexBufferHandle getVertexBuffer() {
    //       return m_vertexBuffer;
    //   }

    //   bgfx::IndexBufferHandle getIndexBuffer() {
    //       return m_indexBuffer;
    //   }


  private:
    bool m_initialized = false;
    GLuint m_vbo, m_vao, m_ebo;
    uint32_t m_numVerticies = 0;
    uint32_t m_numIndices = 0;
};
}  // namespace h_core