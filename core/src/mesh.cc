#include "core/mesh.h"

#include <string>

#include "SDL2/SDL.h"
#include "glad/glad.h"
#include "tiny_gltf.h"

// TODO: move these godless monstrosities to a subf

static h_core::Vertex cubeVertices[] = {
    { h_core::math::Vector3(-1.0f, 1.0f, 1.0f), h_core::math::Vector3(0),
      h_core::math::Vector2(0) },
    { h_core::math::Vector3(1.0f, 1.0f, 1.0f), h_core::math::Vector3(0),
      h_core::math::Vector2(0) },
    { h_core::math::Vector3(-1.0f, -1.0f, 1.0f), h_core::math::Vector3(0),
      h_core::math::Vector2(0) },
    { h_core::math::Vector3(1.0f, -1.0f, 1.0f), h_core::math::Vector3(0),
      h_core::math::Vector2(0) },
    { h_core::math::Vector3(-1.0f, 1.0f, -1.0f), h_core::math::Vector3(0),
      h_core::math::Vector2(0) },
    { h_core::math::Vector3(1.0f, 1.0f, -1.0f), h_core::math::Vector3(0),
      h_core::math::Vector2(0) },
    { h_core::math::Vector3(-1.0f, -1.0f, -1.0f), h_core::math::Vector3(0),
      h_core::math::Vector2(0) },
    { h_core::math::Vector3(1.0f, -1.0f, -1.0f), h_core::math::Vector3(0),
      h_core::math::Vector2(0) },
};
static const uint16_t cubeTriList[] = {
    0, 1, 2,           // 0
    1, 3, 2, 4, 6, 5,  // 2
    5, 6, 7, 0, 2, 4,  // 4
    4, 2, 6, 1, 5, 3,  // 6
    5, 7, 3, 0, 4, 1,  // 8
    4, 5, 1, 2, 3, 6,  // 10
    6, 3, 7,
};


uint32_t h_core::Mesh::initFromYaml(h_core::Assets* assets, YAML::Node yaml) {
    printf("INFO: MODEL: loading model from YAML spec...\n");

    // Set up vertex layout info
    h_core::Vertex::init();

    // Parse YAML
    std::string gltfFilePath = yaml["gltf"].as<std::string>("");
    bool gltfBinaryMode = yaml["gltf_binary"].as<bool>(false);

    if (gltfFilePath.empty()) {
        printf("ERROR: MODEL: no gltf key in model YAML\n");
        return MODEL_INIT_FAIL_BAD_GLTF_FILE_PATH;
    }

    // Load model
    tinygltf::TinyGLTF loader {};
    tinygltf::Model model {};
    std::string errorText {};
    std::string warningText {};

    bool success;
    if (!gltfBinaryMode) {
        success = loader.LoadASCIIFromFile(
            &model, &errorText, &warningText, gltfFilePath);
    }
    else {
        success = loader.LoadBinaryFromFile(
            &model, &errorText, &warningText, gltfFilePath);
    }

    if (!warningText.empty()) {
        printf(("WARN: MODEL: " + warningText + "\n").c_str());
    }

    if (!success) {
        printf(("ERROR: MODEL: " + errorText + "\n").c_str());
        return MODEL_INIT_FAIL_BAD_GLTF;
    }

    // TODO: VERY TESTING. MUCH WOW
    tinygltf::Node node = model.nodes.front();
    tinygltf::Mesh mesh = model.meshes[node.mesh];
    tinygltf::Primitive primitiveInfo = mesh.primitives.front();

    // pos attribute
    uint32_t posAccessorIndex = primitiveInfo.attributes["POSITION"];
    tinygltf::Accessor posAccessor = model.accessors[posAccessorIndex];
    tinygltf::BufferView posBufferView =
        model.bufferViews[posAccessor.bufferView];
    const uint8_t* posBuffer = model.buffers[posBufferView.buffer].data.data();

    // normal attribute
    uint32_t normalAccessorIndex = primitiveInfo.attributes["NORMAL"];
    tinygltf::Accessor normalAccessor = model.accessors[normalAccessorIndex];
    tinygltf::BufferView normalBufferView =
        model.bufferViews[normalAccessor.bufferView];
    const uint8_t* normalBuffer =
        model.buffers[normalBufferView.buffer].data.data();

    // uv attribute
    uint32_t texCoordAccessorIndex = primitiveInfo.attributes["TEXCOORD_0"];
    tinygltf::Accessor texCoordAccessor =
        model.accessors[texCoordAccessorIndex];
    tinygltf::BufferView texCoordBufferView =
        model.bufferViews[texCoordAccessor.bufferView];
    const uint8_t* texCoordBuffer =
        model.buffers[texCoordBufferView.buffer].data.data();

    // load vertex data
    size_t vertexCount = posAccessor.count;
    h_core::Vertex* vertexBuffer = new h_core::Vertex[vertexCount] {};

    for (uint32_t vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++) {
        h_core::Vertex* vertex = &vertexBuffer[vertexIndex];
        vertex->position =
            *(const h_core::math::
                  Vector3*)(&posBuffer
                                [posBufferView.byteOffset +
                                 vertexIndex * sizeof(h_core::math::Vector3)]);
        vertex->normal =
            *(const h_core::math::
                  Vector3*)(&normalBuffer
                                [normalBufferView.byteOffset +
                                 vertexIndex * sizeof(h_core::math::Vector3)]);
        vertex->texCoord =
            *(const h_core::math::
                  Vector2*)(&texCoordBuffer
                                [texCoordBufferView.byteOffset +
                                 vertexIndex * sizeof(h_core::math::Vector2)]);
    }

    // load index buffer
    uint32_t indexBufferAccessorIndex = primitiveInfo.indices;
    tinygltf::Accessor indexBufferAccessor =
        model.accessors[indexBufferAccessorIndex];
    tinygltf::BufferView indexBufferView =
        model.bufferViews[indexBufferAccessor.bufferView];
    std::vector<uint8_t> indexBuffer =
        model.buffers[indexBufferView.buffer].data;


    return 0;
}


void h_core::Mesh::loadModel(
    const uint32_t* vertexCount, const h_core::Vertex* vertexBuffer,
    const uint32_t* inidicesCount, const uint32_t* indexBuffer) {
    if (!m_initialized) {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE, sizeof(h_core::Vertex),
            (const void*)offsetof(h_core::Vertex, position));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE, sizeof(h_core::Vertex),
            (const void*)offsetof(h_core::Vertex, normal));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(
            2, 2, GL_FLOAT, GL_FALSE, sizeof(h_core::Vertex),
            (const void*)offsetof(h_core::Vertex, texCoord));
        glEnableVertexAttribArray(2);

        m_initialized = true;
    }

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    if (vertexCount > 0) {
        glBufferData(
            GL_ARRAY_BUFFER, sizeof(h_core::Vertex) * (*vertexCount),
            vertexBuffer, GL_STATIC_DRAW);
    }

    if (inidicesCount > 0) {
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * (*inidicesCount),
            indexBuffer, GL_STATIC_DRAW);
    }

    m_numVerticies = *vertexCount;
    m_numIndices = *inidicesCount;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}