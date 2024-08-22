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

// What for?
// struct MeshData {
//     std::vector<h_core::Vertex> vertices;
//     std::vector<uint32_t> indicies;
// };

enum class MeshIndexType {
    BYTE,
    SHORT,
    INT
};

class Mesh : public Asset {
  public:
    Mesh() = default;

    uint32_t initFromYaml(h_core::Assets* assets, YAML::Node node);
    void loadModel(
        uint32_t vertexCount, const Vertex* vertexBuffer,
        uint32_t inidicesCount, const void* indexBuffer, MeshIndexType type);

    GLuint getVertexBufferHandle();
    GLuint getIndexBufferHandle();
    GLuint getVertexAttributesHandle();
    size_t getNumVertices();
    size_t getNumIndices();
    MeshIndexType getMeshIndexType();
    uint32_t getPrimitiveMode();

  private:
    // Shouldn't need an initialized flag - should only be initted once!
    // bool m_initialized = false;
    GLuint m_vertexBufferHandle, m_vertexAttributesHandle, m_indexBufferHandle;
    uint32_t m_numVertices = 0;
    uint32_t m_numIndices = 0;
    MeshIndexType m_meshIndexType = MeshIndexType::BYTE;
    uint32_t m_primitiveMode = 4;
};
}  // namespace h_core