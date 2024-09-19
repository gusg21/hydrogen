#include "core/render/modelasset.h"

#include <core/theming/guicolors.h>

#include <string>

#include "SDL2/SDL.h"
#include "glad/glad.h"

#include "core/log.h"
#include "core/render/renderer.h"
#include "core/render/texture.h"
#include "core/runtimesystems.h"
#include "imgui.h"

#define GL_WRAP_MODE_TO_WRAP_MODE(glWrap, mode)                       \
    h_core::render::WrapMode mode = h_core::render::WrapMode::REPEAT; \
    switch (glWrap) {                                                 \
        case GL_REPEAT:                                               \
            mode = h_core::render::WrapMode::REPEAT;                  \
            break;                                                    \
        case GL_MIRRORED_REPEAT:                                      \
            mode = h_core::render::WrapMode::MIRRORED_REPEAT;         \
            break;                                                    \
        case GL_CLAMP_TO_EDGE:                                        \
            mode = h_core::render::WrapMode::CLAMP_TO_EDGE;           \
            break;                                                    \
        case GL_CLAMP_TO_BORDER:                                      \
            mode = h_core::render::WrapMode::CLAMP_TO_BORDER;         \
            break;                                                    \
                                                                      \
        default:                                                      \
            mode = h_core::render::WrapMode::REPEAT;                  \
            break;                                                    \
    }

#define SAMPLE_FILTER_TO_FILTER(sampleFilter, filter) h_core::render::Filter filter = h_core::render::Filter::NEAREST


// TODO: move these godless monstrosities to a subfolder

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

uint32_t h_core::render::ModelAsset::loadMesh(Mesh* out_mesh, const tinygltf::Node& node) {

    tinygltf::Mesh mesh = m_model.meshes[node.mesh];
    tinygltf::Primitive primitiveInfo = mesh.primitives.front();
    out_mesh->primitiveMode = primitiveInfo.mode;
    out_mesh->name = mesh.name;

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
    out_mesh->numVertices = posAccessor.count;
    out_mesh->vertices = new h_core::render::Vertex[out_mesh->numVertices] {};

    for (uint32_t vertexIndex = 0; vertexIndex < out_mesh->numVertices; vertexIndex++) {
        h_core::render::Vertex* vertex = &out_mesh->vertices[vertexIndex];
        vertex->position = reinterpret_cast<const h_core::math::Vector3*>(posBuffer)[vertexIndex];
        vertex->normal = reinterpret_cast<const h_core::math::Vector3*>(normalBuffer)[vertexIndex];
        vertex->texCoord = reinterpret_cast<const h_core::math::Vector2*>(texCoordBuffer)[vertexIndex];
    }

    // load index buffer
    uint32_t indexBufferAccessorIndex = primitiveInfo.indices;
    tinygltf::Accessor indexBufferAccessor = m_model.accessors[indexBufferAccessorIndex];
    tinygltf::BufferView indexBufferView = m_model.bufferViews[indexBufferAccessor.bufferView];
    out_mesh->numIndices = indexBufferAccessor.count;

    // determine index type
    switch (indexBufferAccessor.componentType) {
        case 5121:
            out_mesh->meshIndexType = h_core::render::MeshIndexType::BYTE;
            break;
        case 5123:
            out_mesh->meshIndexType = h_core::render::MeshIndexType::SHORT;
            break;
        case 5125:
            out_mesh->meshIndexType = h_core::render::MeshIndexType::INT;
            break;

        default:
            return MODEL_INIT_FAIL_INVALID_INDEX_DATA_TYPE;
    }

    out_mesh->indices = m_model.buffers[indexBufferView.buffer].data.data() + indexBufferView.byteOffset;

    // TODO: load texture data from glb or gltf file, if specified
    if (primitiveInfo.material != -1) {
        tinygltf::Material material = m_model.materials[primitiveInfo.material];
        HYLOG_INFO("MESH: Loading material %s", material.name.c_str());
        if (material.pbrMetallicRoughness.baseColorTexture.index != -1) {
            tinygltf::Texture texture = m_model.textures[material.pbrMetallicRoughness.baseColorTexture.index];
            HYLOG_INFO("MESH: Loading texture %s ?", texture.name.c_str());

            tinygltf::Sampler sampler = m_model.samplers[texture.sampler];
            tinygltf::Image image = m_model.images[texture.source];

            GL_WRAP_MODE_TO_WRAP_MODE(sampler.wrapS, wrapU);
            GL_WRAP_MODE_TO_WRAP_MODE(sampler.wrapT, wrapV);

            out_mesh->texture.init(
                image.image, image.width, image.height, image.component,
                sampler.magFilter == GL_NEAREST ? h_core::render::Filter::NEAREST : h_core::render::Filter::LINEAR,
                sampler.minFilter == GL_NEAREST ? h_core::render::Filter::NEAREST : h_core::render::Filter::LINEAR,
                wrapU, wrapV);
        }
    }

    return 0;
}

uint32_t h_core::render::ModelAsset::initFromYaml(
    h_core::Assets* assets, const h_core::AssetDescription& desc, const YAML::Node& yaml) {
    h_core::Asset::initFromYaml(assets, desc, yaml);

    HYLOG_INFO("MESH: loading model from YAML spec...\n");

    // Parse YAML
    std::string gltfFilePath = yaml["gltf"].as<std::string>("");
    std::string gltfBasePath = yaml["gltf_base_path"].as<std::string>("");
    bool gltfBinaryMode = yaml["gltf_binary"].as<bool>(false);
    m_isCube = yaml["primitive"].as<bool>(false);
    // std::string baseTexturePath = yaml["texture"]["base"].as<std::string>("");

    if (gltfFilePath.empty()) {
        HYLOG_ERROR("MODEL: no gltf key in model YAML\n");
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

    if (!warningText.empty()) { HYLOG_WARN("MODEL: %s\n", warningText.c_str()); }

    if (!success) {
        HYLOG_ERROR("MODEL: %s\n", errorText.c_str());
        HYLOG_ERROR(
            "MODEL: gltfFilePath = %s, gltfBasePath = %s, gltfBinaryMode = %s\n", gltfFilePath.c_str(),
            gltfBasePath.c_str(), gltfBinaryMode ? "YES" : "NO");
        return MODEL_INIT_FAIL_BAD_GLTF;
    }

    for (uint32_t nodeIndex = 0; nodeIndex < m_model.nodes.size(); nodeIndex++) {
        if (m_model.nodes[nodeIndex].mesh == -1) continue;

        Mesh mesh;
        loadMesh(&mesh, m_model.nodes[nodeIndex]);
        m_meshes.push_back(mesh);
    }

    return 0;
}

uint32_t h_core::render::ModelAsset::precompile(h_core::RuntimeSystems* systems) {
    if (m_isCube) {
        // Just load cube
        HYLOG_DEBUG("MESH: Loading cube primitive mesh\n");
        // loadModel(8, cubeVertices, 36, cubeTriList, h_core::render::MeshIndexType::SHORT,
        // systems->renderer->isGles3());

        return 0;
    }

    for (uint32_t meshIndex = 0; meshIndex < getMeshCount(); meshIndex++) {
        m_meshes[meshIndex].load(systems->renderer->isGles3());

        HYLOG_INFO(
            "MESH: Loaded %zu vertices (%zu indices)\n", m_meshes[meshIndex].numVertices,
            m_meshes[meshIndex].numIndices);
    }

    return 0;
}


void h_core::render::Mesh::load(bool useGles3) {
    // Generate buffers and load attributes
    if (!useGles3) {
        ::glGenVertexArrays(1, &vertexAttributesHandle);
        ::glBindVertexArray(vertexAttributesHandle);
    }

    HYLOG_DEBUG("MESH: using VAO id %d\n", vertexAttributesHandle);

    ::glGenBuffers(1, &vertexBufferHandle);
    ::glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);

    ::glGenBuffers(1, &indexBufferHandle);
    ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferHandle);

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
    ::glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
    ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferHandle);

    // Mark buffers for static drawing (not updated)
    if (numVertices > 0) {
        ::glBufferData(GL_ARRAY_BUFFER, sizeof(h_core::render::Vertex) * numVertices, vertices, GL_STATIC_DRAW);
    }

    if (numIndices > 0) {
        // Determine index type size
        size_t indexTypeSize = 0;
        switch (meshIndexType) {
            case MeshIndexType::BYTE:
                indexTypeSize = sizeof(uint8_t);
                break;
            case MeshIndexType::SHORT:
                indexTypeSize = sizeof(uint16_t);
                break;
            case MeshIndexType::INT:
                indexTypeSize = sizeof(uint32_t);
                break;
            default:
                HYLOG_ERROR("Undefined mesh index type value\n");
                indexTypeSize = sizeof(uint16_t);
                break;
        }

        ::glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexTypeSize * numIndices, indices, GL_STATIC_DRAW);
    }

    // Clean up
    if (!useGles3) { ::glBindVertexArray(0); }
    ::glBindBuffer(GL_ARRAY_BUFFER, 0);
    ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

h_core::render::Mesh* h_core::render::ModelAsset::getMeshes() {
    return m_meshes.data();
}
uint32_t h_core::render::ModelAsset::getMeshCount() {
    return m_meshes.size();
}

std::vector<uint8_t>* h_core::render::ModelAsset::toPacked() {
    HYLOG_DEBUG("MESH: Packing mesh...");

    /*uint32_t indexSize = 1;
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

    // Try to keep in sync with actual byte requirements
    size_t byteLength =
        sizeof(uint32_t) * 3 + sizeof(h_core::render::Vertex) * m_numVertices + indexSize * m_numIndices;
    std::vector<uint8_t>* bytes = new std::vector<uint8_t>();
    bytes->resize(byteLength);
    uint8_t* writeHead = bytes->data();

    // # vertices
    memcpy(writeHead, (uint8_t*)&m_numVertices, sizeof(uint32_t));
    writeHead += sizeof(uint32_t);

    // vertices
    memcpy(writeHead, (uint8_t*)m_vertices, sizeof(h_core::render::Vertex) * m_numVertices);
    writeHead += sizeof(h_core::render::Vertex) * m_numVertices;

    // index size
    memcpy(writeHead, (uint8_t*)&indexSize, sizeof(uint32_t));
    writeHead += sizeof(uint32_t);

    // # indices
    memcpy(writeHead, (uint8_t*)&m_numIndices, sizeof(uint32_t));
    writeHead += sizeof(uint32_t);

    // indices
    memcpy(writeHead, (uint8_t*)m_indices, indexSize * m_numIndices);
    writeHead += indexSize * m_numIndices;

    assert(writeHead - bytes->data() == byteLength);

    return bytes;*/
    return nullptr;
}

void h_core::render::ModelAsset::fromPacked(const void* data, size_t length) {
    /*const uint8_t* bytes = (const uint8_t*)data;
    const uint8_t* readHead = bytes;

    HYLOG_DEBUG("MESH: loading from packed (%zu bytes)\n", length);
    HYLOG_DEBUG("MESH: first byte %x\n", readHead[0]);

    // # vertices
    uint32_t numVertices = *(uint32_t*)(readHead);
    HYLOG_DEBUG("MESH: # of vertices: %d\n", numVertices);
    readHead += sizeof(uint32_t);

    // vertices data
    m_vertices = new h_core::render::Vertex[numVertices];
    h_core::render::Vertex* newVerts = (h_core::render::Vertex*)readHead;
    for (uint32_t vertexIndex = 0; vertexIndex < numVertices; vertexIndex++) {
        m_vertices[vertexIndex] = newVerts[vertexIndex];
    }
    /*memcpy_s(
        m_vertices, numVertices * sizeof(h_core::render::Vertex), ,
        numVertices * sizeof(h_core::render::Vertex));#1#
    readHead += numVertices * sizeof(h_core::render::Vertex);

    // index type
    uint32_t indexTypeSize = *(uint32_t*)(readHead);
    switch (indexTypeSize) {
        case 1:
            m_meshIndexType = MeshIndexType::BYTE;
            break;
        case 2:
            m_meshIndexType = MeshIndexType::SHORT;
            break;
        case 4:
            m_meshIndexType = MeshIndexType::INT;
            break;
        default:
            HYLOG_ERROR("MESH: Invalid packed mesh index type %d", indexTypeSize);
            return;
    }
    readHead += sizeof(uint32_t);

    // # indices
    uint32_t numIndices = *(uint32_t*)(readHead);
    HYLOG_DEBUG("MESH: # of indices: %d\n", numIndices);
    readHead += sizeof(uint32_t);

    // indices data
    switch (m_meshIndexType) {
        case MeshIndexType::BYTE:
            m_indices = new uint8_t[numIndices];
            break;
        case MeshIndexType::SHORT:
            m_indices = new uint16_t[numIndices];
            break;
        case MeshIndexType::INT:
            m_indices = new uint32_t[numIndices];
            break;
    }
    memcpy(m_indices, readHead, numIndices * indexTypeSize);
    readHead += numIndices * indexTypeSize;

    // Store data
    m_numVertices = numVertices;
    m_numIndices = numIndices;
    m_primitiveMode = GL_TRIANGLES;
    m_isCube = false;*/
}

void h_core::render::ModelAsset::doGUI() {
    //    Asset::doGUI();

    for (Mesh mesh : m_meshes) {
        ImGui::TextColored(IMGUI_COLOR_GOOD, "Name: %s", mesh.name.c_str());
        ImGui::Text("Vertex Count: %d", mesh.numVertices);
        ImGui::Text("Index Count: %d", mesh.numIndices);
        const char* indexTypeName;
        switch (mesh.meshIndexType) {
            case MeshIndexType::BYTE:
                indexTypeName = "BYTE";
                break;
            case MeshIndexType::SHORT:
                indexTypeName = "SHORT";
                break;
            case MeshIndexType::INT:
                indexTypeName = "INT";
                break;
            default:
                indexTypeName = "WHAT";
                break;
        }
        ImGui::Text("Index Type: %s", indexTypeName);
        ImGui::Text("Is Cube?: %s", m_isCube ? "YES" : "NO");
        ImGui::Text("IBO: %d", mesh.indexBufferHandle);
        ImGui::Text("VBO: %d", mesh.vertexBufferHandle);
        ImGui::Text("VAO: %d", mesh.vertexAttributesHandle);
        ImGui::Text("Primitive Mode: %d", mesh.primitiveMode);

        ImGui::Separator();
    }
}
