#pragma once

#include "glad/glad.h"
#include "tiny_gltf.h"

#include "core/asset.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"

#define MODEL_COMPONENT_BITMASK                 (1 << 1)
#define MODEL_INIT_FAIL_BAD_GLTF_FILE_PATH      1
#define MODEL_INIT_FAIL_BAD_GLTF                2
#define MODEL_INIT_FAIL_INVALID_INDEX_DATA_TYPE 3

namespace h_core {
namespace render {
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

enum class MeshIndexType { BYTE, SHORT, INT };

class MeshAsset : public Asset {
  public:
    MeshAsset() = default;

    uint32_t initFromYaml(
        h_core::Assets* assets,
        YAML::Node node) override;
    uint32_t precompile(h_core::Systems* systems) override;

    void loadModel(
        uint32_t vertexCount, const Vertex* vertexBuffer,
        uint32_t inidicesCount, const void* indexBuffer, MeshIndexType type);

    [[nodiscard]] GLuint getVertexBufferHandle() const;
    [[nodiscard]] GLuint getIndexBufferHandle() const;
    [[nodiscard]] GLuint getVertexAttributesHandle() const;
    [[nodiscard]] size_t getNumVertices() const;
    [[nodiscard]] size_t getNumIndices() const;
    [[nodiscard]] MeshIndexType getMeshIndexType() const;
    [[nodiscard]] uint32_t getPrimitiveMode() const;

    HYASSET(3);

  private:
    // Shouldn't need an initialized flag - should only be initted once!
    // bool m_initialized = false;
    tinygltf::Model m_model {};
    GLuint m_vertexBufferHandle = 0, m_vertexAttributesHandle = 0, m_indexBufferHandle = 0;
    uint32_t m_numVertices = 0;
    uint32_t m_numIndices = 0;
    h_core::render::MeshIndexType m_meshIndexType =
        h_core::render::MeshIndexType::BYTE;
    uint32_t m_primitiveMode = 4;
};
}  // namespace render
}  // namespace h_core