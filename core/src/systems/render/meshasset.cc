#include "core/systems/render/meshasset.h"

#include <string>

#include "SDL2/SDL.h"
#include "glad/glad.h"

#include "core/systems/render/renderer.h"

// TODO: move these godless monstrosities to a subf

static h_core::render::Vertex cubeVertices[] = {
    { h_core::math::Vector3(-1.0f, 1.0f, 1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
    { h_core::math::Vector3(1.0f, 1.0f, 1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
    { h_core::math::Vector3(-1.0f, -1.0f, 1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
    { h_core::math::Vector3(1.0f, -1.0f, 1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
    { h_core::math::Vector3(-1.0f, 1.0f, -1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
    { h_core::math::Vector3(1.0f, 1.0f, -1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
    { h_core::math::Vector3(-1.0f, -1.0f, -1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
    { h_core::math::Vector3(1.0f, -1.0f, -1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
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


uint32_t h_core::render::MeshAsset::initFromYaml(h_core::Assets* assets, YAML::Node yaml) {
    ::SDL_Log("INFO: MESH: loading model from YAML spec...\n");

    // Parse YAML
    std::string gltfFilePath = yaml["gltf"].as<std::string>("");
    std::string gltfBasePath = yaml["gltf_base_path"].as<std::string>("");
    bool gltfBinaryMode = yaml["gltf_binary"].as<bool>(false);
    m_isCube = yaml["primitive"].as<bool>(false);

    if (gltfFilePath.empty()) {
        ::SDL_Log("ERROR: MODEL: no gltf key in model YAML\n");
        return MODEL_INIT_FAIL_BAD_GLTF_FILE_PATH;
    }

    // Load model
    tinygltf::TinyGLTF loader {};
    std::string errorText {};
    std::string warningText {};

    bool success;
    if (!gltfBinaryMode) {
        size_t gltfTextLength;
        const char* gltfText = (const char*)SDL_LoadFile(gltfFilePath.c_str(), &gltfTextLength);
        success =
            loader.LoadASCIIFromString(&m_model, &errorText, &warningText, gltfText, gltfTextLength, gltfBasePath);
    }
    else {
        size_t glbDataLength;
        const uint8_t* glbData = (const uint8_t*)SDL_LoadFile(gltfFilePath.c_str(), &glbDataLength);
        success = loader.LoadBinaryFromMemory(&m_model, &errorText, &warningText, glbData, glbDataLength, gltfBasePath);
    }

    if (!warningText.empty()) { ::SDL_Log("WARN: MODEL: %s\n", warningText.c_str()); }

    if (!success) {
        ::SDL_Log("ERROR: MODEL: %s\n", errorText.c_str());
        ::SDL_Log(
            "ERROR: MODEL: gltfFilePath = %s, gltfBasePath = %s, gltfBinaryMode = %s\n", gltfFilePath.c_str(),
            gltfBasePath.c_str(), gltfBinaryMode ? "YES" : "NO");
        return MODEL_INIT_FAIL_BAD_GLTF;
    }

    // TODO: VERY TESTING. MUCH WOW
    tinygltf::Node node = m_model.nodes.front();
    tinygltf::Mesh mesh = m_model.meshes[node.mesh];
    tinygltf::Primitive primitiveInfo = mesh.primitives.front();
    m_primitiveMode = primitiveInfo.mode;

    // pos attribute
    uint32_t posAccessorIndex = primitiveInfo.attributes["POSITION"];
    tinygltf::Accessor posAccessor = m_model.accessors[posAccessorIndex];
    tinygltf::BufferView posBufferView = m_model.bufferViews[posAccessor.bufferView];
    const uint8_t* posBuffer = m_model.buffers[posBufferView.buffer].data.data() + posBufferView.byteOffset;

    // normal attribute
    uint32_t normalAccessorIndex = primitiveInfo.attributes["NORMAL"];
    tinygltf::Accessor normalAccessor = m_model.accessors[normalAccessorIndex];
    tinygltf::BufferView normalBufferView = m_model.bufferViews[normalAccessor.bufferView];
    const uint8_t* normalBuffer = m_model.buffers[normalBufferView.buffer].data.data() + normalBufferView.byteOffset;

    // uv attribute
    uint32_t texCoordAccessorIndex = primitiveInfo.attributes["TEXCOORD_0"];
    tinygltf::Accessor texCoordAccessor = m_model.accessors[texCoordAccessorIndex];
    tinygltf::BufferView texCoordBufferView = m_model.bufferViews[texCoordAccessor.bufferView];
    const uint8_t* texCoordBuffer =
        m_model.buffers[texCoordBufferView.buffer].data.data() + texCoordBufferView.byteOffset;

    // load vertex data
    m_numVertices = posAccessor.count;
    m_vertices = new h_core::render::Vertex[m_numVertices] {};

    for (uint32_t vertexIndex = 0; vertexIndex < m_numVertices; vertexIndex++) {
        h_core::render::Vertex* vertex = &m_vertices[vertexIndex];
        vertex->position = reinterpret_cast<const h_core::math::Vector3*>(posBuffer)[vertexIndex];
        vertex->normal = reinterpret_cast<const h_core::math::Vector3*>(normalBuffer)[vertexIndex];
        vertex->texCoord = reinterpret_cast<const h_core::math::Vector2*>(texCoordBuffer)[vertexIndex];
    }

    // load index buffer
    uint32_t indexBufferAccessorIndex = primitiveInfo.indices;
    tinygltf::Accessor indexBufferAccessor = m_model.accessors[indexBufferAccessorIndex];
    tinygltf::BufferView indexBufferView = m_model.bufferViews[indexBufferAccessor.bufferView];
    m_numIndices = indexBufferAccessor.count;

    // determine index type
    switch (indexBufferAccessor.componentType) {
        case 5121:
            m_meshIndexType = h_core::render::MeshIndexType::BYTE;
            break;
        case 5123:
            m_meshIndexType = h_core::render::MeshIndexType::SHORT;
            break;
        case 5125:
            m_meshIndexType = h_core::render::MeshIndexType::INT;
            break;

        default:
            return MODEL_INIT_FAIL_INVALID_INDEX_DATA_TYPE;
    }

    m_indices = m_model.buffers[indexBufferView.buffer].data.data() + indexBufferView.byteOffset;

    return 0;
}

uint32_t h_core::render::MeshAsset::precompile(h_core::RuntimeSystems* systems) {
    if (m_isCube) {
        // Just load cube
        ::SDL_Log("DEBUG: MESH: Loading cube primitive mesh\n");
        loadModel(8, cubeVertices, 36, cubeTriList, h_core::render::MeshIndexType::SHORT, systems->renderer->isGles3());

        return 0;
    }

    loadModel(
        m_numVertices, m_vertices, m_numIndices, m_indices, m_meshIndexType, systems->renderer->isGles3());

    ::SDL_Log("MESH: INFO: Loaded %zu vertices (%zu indices)\n", m_numVertices, m_numIndices);

    return 0;
}

void h_core::render::MeshAsset::loadModel(
    uint32_t vertexBufferCount, const h_core::render::Vertex* vertexBuffer, uint32_t inidicesCount,
    const void* indexBuffer, MeshIndexType indexType, bool useGles3) {
    // Generate buffers and load attributes
    if (!useGles3) {
        ::glGenVertexArrays(1, &m_vertexAttributesHandle);
        ::glBindVertexArray(m_vertexAttributesHandle);
    }

    ::SDL_Log("DEBUG: MESH: using VAO id %d\n", m_vertexAttributesHandle);

    ::glGenBuffers(1, &m_vertexBufferHandle);
    ::glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);

    ::glGenBuffers(1, &m_indexBufferHandle);
    ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);

    ::glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(h_core::render::Vertex),
        (const void*)offsetof(h_core::render::Vertex, position));
    ::glEnableVertexAttribArray(0);

    ::glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(h_core::render::Vertex),
        (const void*)offsetof(h_core::render::Vertex, normal));
    ::glEnableVertexAttribArray(1);

    ::glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(h_core::render::Vertex),
        (const void*)offsetof(h_core::render::Vertex, texCoord));
    ::glEnableVertexAttribArray(2);

    // Bind the vertex and index buffers to this VAO
    ::glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
    ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);

    // Mark buffers for static drawing (not updated)
    if (vertexBufferCount > 0) {
        ::glBufferData(
            GL_ARRAY_BUFFER, sizeof(h_core::render::Vertex) * vertexBufferCount, vertexBuffer, GL_STATIC_DRAW);
    }

    if (inidicesCount > 0) {
        // Determine index type size
        size_t indexTypeSize = 0;
        switch (indexType) {
            case MeshIndexType::BYTE:
                indexTypeSize = sizeof(unsigned char);
                break;
            case MeshIndexType::SHORT:
                indexTypeSize = sizeof(unsigned short);
                break;
            case MeshIndexType::INT:
                indexTypeSize = sizeof(unsigned int);
                break;
            default:
                ::SDL_Log("Undefined mesh index type value!! What!!!\n");
                break;
        }

        ::glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexTypeSize * inidicesCount, indexBuffer, GL_STATIC_DRAW);
    }

    // Store buffer sizes
    m_numVertices = vertexBufferCount;
    m_numIndices = inidicesCount;
    m_meshIndexType = indexType;

    // Clean up
    if (!useGles3) { ::glBindVertexArray(0); }
    ::glBindBuffer(GL_ARRAY_BUFFER, 0);
    ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


GLuint h_core::render::MeshAsset::getIndexBufferHandle() const {
    return m_indexBufferHandle;
}

GLuint h_core::render::MeshAsset::getVertexAttributesHandle() const {
    return m_vertexAttributesHandle;
}

GLuint h_core::render::MeshAsset::getVertexBufferHandle() const {
    return m_vertexBufferHandle;
}

size_t h_core::render::MeshAsset::getNumVertices() const {
    return m_numVertices;
}

size_t h_core::render::MeshAsset::getNumIndices() const {
    return m_numIndices;
}

h_core::render::MeshIndexType h_core::render::MeshAsset::getMeshIndexType() const {
    return m_meshIndexType;
}

uint32_t h_core::render::MeshAsset::getPrimitiveMode() const {
    return m_primitiveMode;
}

std::vector<char>* h_core::render::MeshAsset::toPacked() {
    std::vector<char>* bytes = new std::vector<char>();
    bytes->reserve(m_numVertices + m_numIndices + 2); // Try to keep in sync with actual byte requirements

    bytes->insert(bytes->end(), (char*)&m_numVertices, (char*)&m_numVertices + sizeof(uint32_t));
    bytes->insert(bytes->end(), (char*)m_vertices, (char*)(m_vertices) + (m_numVertices * sizeof(h_core::render::Vertex)));
    bytes->insert(bytes->end(), (char*)&m_numIndices, (char*)&m_numIndices + sizeof(uint32_t));
    uint32_t indexSize = 1;
    switch (m_meshIndexType) {
        case MeshIndexType::BYTE:
            indexSize = 1;
            break;
        case MeshIndexType::SHORT:
            indexSize = 2;
            break;
        case MeshIndexType::INT:
            indexSize = 4;
            break;
    }
    bytes->insert(bytes->end(), (char*)m_indices, (char*)(m_indices) + (m_numIndices * indexSize));

    return bytes;
}

void h_core::render::MeshAsset::fromPacked(const void* data, size_t length) {
    const char* bytes = (const char*)data;

    SDL_Log("INFO: MESH: loading from packed (%zu bytes)\n", length);
    SDL_Log("INFO: MESH: first byte %x\n", bytes[0]);

    uint32_t numVertices = *(uint32_t*)(bytes + 0);
    SDL_Log("INFO: MESH: # of vertices: %d\n", numVertices);

    m_vertices = new h_core::render::Vertex[numVertices];
    memcpy(m_vertices, bytes + sizeof(uint32_t), numVertices * sizeof(h_core::render::Vertex));

    uint32_t numIndices = *(uint32_t*)(bytes + sizeof(uint32_t) + numVertices * sizeof(h_core::render::Vertex));
    SDL_Log("INFO: MESH: # of indices: %d\n", numIndices);

    m_indices = new uint16_t[numIndices]; // TODO: use actual index type
    memcpy(m_indices,
           bytes + sizeof(uint32_t) + numVertices * sizeof(h_core::render::Vertex) + sizeof(uint32_t),
           numIndices * sizeof(uint16_t));

    m_numVertices = numVertices;
    m_numIndices = numIndices;
    m_meshIndexType = MeshIndexType::SHORT;
    m_primitiveMode = GL_TRIANGLES;
    m_isCube = false;
}
