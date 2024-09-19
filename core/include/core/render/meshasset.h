#pragma once

#include "glad/glad.h"
#include "tiny_gltf.h"

#include "core/asset.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/render/texture.h"

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

    uint32_t initFromYaml(h_core::Assets* assets, const h_core::AssetDescription& desc, const YAML::Node& yaml) override;
    uint32_t precompile(h_core::RuntimeSystems* systems) override;
    std::vector<uint8_t>* toPacked() override;
    void fromPacked(const void* data, size_t length) override;
    void doGUI() override;

    void loadModel(
        uint32_t vertexCount, const Vertex* vertexBuffer, uint32_t inidicesCount, const void* indexBuffer,
        MeshIndexType type, bool useGles3);

    [[nodiscard]] GLuint getVertexBufferHandle() const;
    [[nodiscard]] GLuint getIndexBufferHandle() const;
    [[nodiscard]] GLuint getVertexAttributesHandle() const;
    [[nodiscard]] size_t getNumVertices() const;
    [[nodiscard]] size_t getNumIndices() const;
    [[nodiscard]] MeshIndexType getMeshIndexType() const;
    [[nodiscard]] uint32_t getPrimitiveMode() const;
    [[nodiscard]] uint32_t getTexture() const;

    HYASSET(3);

  private:
    tinygltf::Model m_model {};
    bool m_isCube = false;

    h_core::render::Texture m_texture {};

    GLuint m_vertexBufferHandle = 0, m_vertexAttributesHandle = 0, m_indexBufferHandle = 0;
    uint32_t m_numVertices = 0;
    uint32_t m_numIndices = 0;
    h_core::render::Vertex* m_vertices = nullptr;
    void* m_indices = nullptr;
    h_core::render::MeshIndexType m_meshIndexType = h_core::render::MeshIndexType::BYTE;
    uint32_t m_primitiveMode = 4;

};
}  // namespace render
}  // namespace h_core