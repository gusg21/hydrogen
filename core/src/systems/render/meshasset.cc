#include "core/systems/render/meshasset.h"

#include <string>

#include "SDL2/SDL.h"
#include "glad/glad.h"
#include "tiny_gltf.h"

// TODO: move these godless monstrosities to a subf

static h_core::render::Vertex cubeVertices[] = {
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


uint32_t h_core::render::MeshAsset::initFromYaml(
    h_core::Assets* assets, h_core::Systems* systems, YAML::Node yaml) {
    ::printf("INFO: MESH: loading model from YAML spec...\n");

    if (yaml["primitive"].as<bool>(false)) {
        // Just load cube
        ::printf("DEBUG: MESH: Loading cube primitive mesh\n");
        loadModel(
            8, cubeVertices, 36, cubeTriList,
            h_core::render::MeshIndexType::SHORT);

        return 0;
    }

    // Parse YAML
    std::string gltfFilePath = yaml["gltf"].as<std::string>("");
    bool gltfBinaryMode = yaml["gltf_binary"].as<bool>(false);

    if (gltfFilePath.empty()) {
        ::printf("ERROR: MODEL: no gltf key in model YAML\n");
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
        ::printf(("WARN: MODEL: " + warningText + "\n").c_str());
    }

    if (!success) {
        ::printf(("ERROR: MODEL: " + errorText + "\n").c_str());
        return MODEL_INIT_FAIL_BAD_GLTF;
    }

    // TODO: VERY TESTING. MUCH WOW
    tinygltf::Node node = model.nodes.front();
    tinygltf::Mesh mesh = model.meshes[node.mesh];
    tinygltf::Primitive primitiveInfo = mesh.primitives.front();
    m_primitiveMode = primitiveInfo.mode;

    // pos attribute
    uint32_t posAccessorIndex = primitiveInfo.attributes["POSITION"];
    tinygltf::Accessor posAccessor = model.accessors[posAccessorIndex];
    tinygltf::BufferView posBufferView =
        model.bufferViews[posAccessor.bufferView];
    const uint8_t* posBuffer = model.buffers[posBufferView.buffer].data.data() +
                               posBufferView.byteOffset;

    // normal attribute
    uint32_t normalAccessorIndex = primitiveInfo.attributes["NORMAL"];
    tinygltf::Accessor normalAccessor = model.accessors[normalAccessorIndex];
    tinygltf::BufferView normalBufferView =
        model.bufferViews[normalAccessor.bufferView];
    const uint8_t* normalBuffer =
        model.buffers[normalBufferView.buffer].data.data() +
        normalBufferView.byteOffset;

    // uv attribute
    uint32_t texCoordAccessorIndex = primitiveInfo.attributes["TEXCOORD_0"];
    tinygltf::Accessor texCoordAccessor =
        model.accessors[texCoordAccessorIndex];
    tinygltf::BufferView texCoordBufferView =
        model.bufferViews[texCoordAccessor.bufferView];
    const uint8_t* texCoordBuffer =
        model.buffers[texCoordBufferView.buffer].data.data() +
        texCoordBufferView.byteOffset;

    // load vertex data
    size_t vertexBufferCount = posAccessor.count;
    h_core::render::Vertex* vertexBuffer =
        new h_core::render::Vertex[vertexBufferCount] {};

    for (uint32_t vertexIndex = 0; vertexIndex < vertexBufferCount;
         vertexIndex++) {
        h_core::render::Vertex* vertex = &vertexBuffer[vertexIndex];
        vertex->position = reinterpret_cast<const h_core::math::Vector3*>(
            posBuffer)[vertexIndex];
        vertex->normal = reinterpret_cast<const h_core::math::Vector3*>(
            normalBuffer)[vertexIndex];
        vertex->texCoord = reinterpret_cast<const h_core::math::Vector2*>(
            texCoordBuffer)[vertexIndex];
    }

    // load index buffer
    uint32_t indexBufferAccessorIndex = primitiveInfo.indices;
    tinygltf::Accessor indexBufferAccessor =
        model.accessors[indexBufferAccessorIndex];
    tinygltf::BufferView indexBufferView =
        model.bufferViews[indexBufferAccessor.bufferView];
    size_t indexBufferCount = indexBufferAccessor.count;

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

    loadModel(
        vertexBufferCount, vertexBuffer, indexBufferCount,
        model.buffers[indexBufferView.buffer].data.data() +
            indexBufferView.byteOffset,
        m_meshIndexType);

    return 0;
}
uint32_t h_core::render::MeshAsset::initFromFile(
    h_core::Assets* assets, h_core::Systems* systems, std::string filepath) {
        if (filepath.empty()) {
        ::printf("ERROR: MODEL: no gltf key in model YAML\n");
        return MODEL_INIT_FAIL_BAD_GLTF_FILE_PATH;
    }

    //Hacky solution, but it was the only thing I thought of
    size_t dotIndex = filepath.find_first_of('.');
    bool gltfBinaryMode = false;

    if(dotIndex != std::string::npos && dotIndex < filepath.size() - 1) {
        std::string fileType = filepath.substr(dotIndex + 1);

        if (fileType == "glb") { gltfBinaryMode = true;}
    }

    // Load model
    tinygltf::TinyGLTF loader {};
    tinygltf::Model model {};
    std::string errorText {};
    std::string warningText {};

    bool success;
    if (!gltfBinaryMode) { //TODO: figure out if file is glb or gltf
        success = loader.LoadASCIIFromFile(
            &model, &errorText, &warningText, filepath);
    }
    else {
        success = loader.LoadBinaryFromFile(
            &model, &errorText, &warningText, filepath);
    }

    if (!warningText.empty()) {
        ::printf(("WARN: MODEL: " + warningText + "\n").c_str());
    }

    if (!success) {
        ::printf(("ERROR: MODEL: " + errorText + "\n").c_str());
        return MODEL_INIT_FAIL_BAD_GLTF;
    }

    // TODO: VERY TESTING. MUCH WOW
    tinygltf::Node node = model.nodes.front();
    tinygltf::Mesh mesh = model.meshes[node.mesh];
    tinygltf::Primitive primitiveInfo = mesh.primitives.front();
    m_primitiveMode = primitiveInfo.mode;

    // pos attribute
    uint32_t posAccessorIndex = primitiveInfo.attributes["POSITION"];
    tinygltf::Accessor posAccessor = model.accessors[posAccessorIndex];
    tinygltf::BufferView posBufferView =
        model.bufferViews[posAccessor.bufferView];
    const uint8_t* posBuffer = model.buffers[posBufferView.buffer].data.data() +
                               posBufferView.byteOffset;

    // normal attribute
    uint32_t normalAccessorIndex = primitiveInfo.attributes["NORMAL"];
    tinygltf::Accessor normalAccessor = model.accessors[normalAccessorIndex];
    tinygltf::BufferView normalBufferView =
        model.bufferViews[normalAccessor.bufferView];
    const uint8_t* normalBuffer =
        model.buffers[normalBufferView.buffer].data.data() +
        normalBufferView.byteOffset;

    // uv attribute
    uint32_t texCoordAccessorIndex = primitiveInfo.attributes["TEXCOORD_0"];
    tinygltf::Accessor texCoordAccessor =
        model.accessors[texCoordAccessorIndex];
    tinygltf::BufferView texCoordBufferView =
        model.bufferViews[texCoordAccessor.bufferView];
    const uint8_t* texCoordBuffer =
        model.buffers[texCoordBufferView.buffer].data.data() +
        texCoordBufferView.byteOffset;

    // load vertex data
    size_t vertexBufferCount = posAccessor.count;
    h_core::render::Vertex* vertexBuffer =
        new h_core::render::Vertex[vertexBufferCount] {};

    for (uint32_t vertexIndex = 0; vertexIndex < vertexBufferCount;
         vertexIndex++) {
        h_core::render::Vertex* vertex = &vertexBuffer[vertexIndex];
        vertex->position = reinterpret_cast<const h_core::math::Vector3*>(
            posBuffer)[vertexIndex];
        vertex->normal = reinterpret_cast<const h_core::math::Vector3*>(
            normalBuffer)[vertexIndex];
        vertex->texCoord = reinterpret_cast<const h_core::math::Vector2*>(
            texCoordBuffer)[vertexIndex];
    }

    // load index buffer
    uint32_t indexBufferAccessorIndex = primitiveInfo.indices;
    tinygltf::Accessor indexBufferAccessor =
        model.accessors[indexBufferAccessorIndex];
    tinygltf::BufferView indexBufferView =
        model.bufferViews[indexBufferAccessor.bufferView];
    size_t indexBufferCount = indexBufferAccessor.count;

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

    loadModel(
        vertexBufferCount, vertexBuffer, indexBufferCount,
        model.buffers[indexBufferView.buffer].data.data() +
            indexBufferView.byteOffset,
        m_meshIndexType);

    return 0;
}

void h_core::render::MeshAsset::loadModel(
    uint32_t vertexBufferCount, const h_core::render::Vertex* vertexBuffer,
    uint32_t inidicesCount, const void* indexBuffer, MeshIndexType indexType) {
    // Generate buffers and load attributes
    ::glGenVertexArrays(1, &m_vertexAttributesHandle);
    ::glBindVertexArray(m_vertexAttributesHandle);

    ::printf("DEBUG: MESH: using VAO id %d\n", m_vertexAttributesHandle);

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
            GL_ARRAY_BUFFER, sizeof(h_core::render::Vertex) * vertexBufferCount,
            vertexBuffer, GL_STATIC_DRAW);
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
                ::printf("Undefined mesh index type value!! What!!!\n");
                break;
        }

       ::glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, indexTypeSize * inidicesCount, indexBuffer,
            GL_STATIC_DRAW);
    }

    // Store buffer sizes
    m_numVertices = vertexBufferCount;
    m_numIndices = inidicesCount;
    m_meshIndexType = indexType;

    // Clean up
    ::glBindVertexArray(0);
    ::glBindBuffer(GL_ARRAY_BUFFER, 0);
    ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


GLuint h_core::render::MeshAsset::getIndexBufferHandle() {
    return m_indexBufferHandle;
}

GLuint h_core::render::MeshAsset::getVertexAttributesHandle() {
    return m_vertexAttributesHandle;
}

GLuint h_core::render::MeshAsset::getVertexBufferHandle() {
    return m_vertexBufferHandle;
}

size_t h_core::render::MeshAsset::getNumVertices() {
    return m_numVertices;
}

size_t h_core::render::MeshAsset::getNumIndices() {
    return m_numIndices;
}

h_core::render::MeshIndexType h_core::render::MeshAsset::getMeshIndexType() {
    return m_meshIndexType;
}

uint32_t h_core::render::MeshAsset::getPrimitiveMode() {
    return m_primitiveMode;
}