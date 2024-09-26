#include "core/render/modelasset.h"

#include <core/theming/guicolors.h>

#include <string>

#include "SDL2/SDL.h"
#ifndef HYCORE_HEADLESS
#include "glad/glad.h"
#endif

#include "core/log.h"
#include "core/render/renderer.h"
#include "core/render/texture.h"
#include "core/runtimesystems.h"
#include "imgui.h"

#define GL_WRAP_MODE_TO_WRAP_MODE(glWrap, mode)                       \
    h_core::render::WrapMode mode = h_core::render::WrapMode::REPEAT; \
    switch (glWrap) {                                                 \
        case TINYGLTF_TEXTURE_WRAP_REPEAT:                            \
            mode = h_core::render::WrapMode::REPEAT;                  \
            break;                                                    \
        case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:                   \
            mode = h_core::render::WrapMode::MIRRORED_REPEAT;         \
            break;                                                    \
        case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:                     \
            mode = h_core::render::WrapMode::CLAMP_TO_EDGE;           \
            break;                                                    \
            /*case GL_CLAMP_TO_BORDER:*/                              \
            /*    mode = h_core::render::WrapMode::CLAMP_TO_BORDER;*/ \
            /*    break;*/                                            \
                                                                      \
        default:                                                      \
            mode = h_core::render::WrapMode::REPEAT;                  \
            break;                                                    \
    }

#define SAMPLE_FILTER_TO_FILTER(sampleFilter, filter) h_core::render::Filter filter = h_core::render::Filter::NEAREST


// TODO: move these godless monstrosities to a subfolder

// static h_core::render::Vertex cubeVertices[] = {
//     { h_core::math::Vector3(-1.0f, 1.0f, 1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
//     { h_core::math::Vector3(1.0f, 1.0f, 1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
//     { h_core::math::Vector3(-1.0f, -1.0f, 1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
//     { h_core::math::Vector3(1.0f, -1.0f, 1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
//     { h_core::math::Vector3(-1.0f, 1.0f, -1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
//     { h_core::math::Vector3(1.0f, 1.0f, -1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
//     { h_core::math::Vector3(-1.0f, -1.0f, -1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
//     { h_core::math::Vector3(1.0f, -1.0f, -1.0f), h_core::math::Vector3(0), h_core::math::Vector2(0) },
// };
//
// static const uint16_t cubeTriList[] = {
//     0, 1, 2,           // 0
//     1, 3, 2, 4, 6, 5,  // 2
//     5, 6, 7, 0, 2, 4,  // 4
//     4, 2, 6, 1, 5, 3,  // 6
//     5, 7, 3, 0, 4, 1,  // 8
//     4, 5, 1, 2, 3, 6,  // 10
//     6, 3, 7,
// };

uint32_t h_core::render::Mesh::initFromNode(const tinygltf::Model& model, const tinygltf::Node& node) {
    tinygltf::Mesh mesh = model.meshes[node.mesh];
    tinygltf::Primitive primitiveInfo = mesh.primitives.front();
    if (mesh.primitives.size() > 1) {
        HYLOG_WARN("MESH: More than one primitive per mesh? Unhandled.");
        return 1;
    }
    primitiveMode = static_cast<MeshPrimitiveMode>(primitiveInfo.mode);
    name = mesh.name;

    // pos attribute
    uint32_t posAccessorIndex = primitiveInfo.attributes["POSITION"];
    tinygltf::Accessor posAccessor = model.accessors[posAccessorIndex];
    tinygltf::BufferView posBufferView = model.bufferViews[posAccessor.bufferView];
    size_t posBufferByteStride;
    if (posBufferView.byteStride == 0) { posBufferByteStride = sizeof(h_core::math::Vector3); }
    else { posBufferByteStride = posBufferView.byteStride; }
    const uint8_t* posBuffer =
        model.buffers[posBufferView.buffer].data.data() + posBufferView.byteOffset + posAccessor.byteOffset;

    // normal attribute
    uint32_t normalAccessorIndex = primitiveInfo.attributes["NORMAL"];
    tinygltf::Accessor normalAccessor = model.accessors[normalAccessorIndex];
    tinygltf::BufferView normalBufferView = model.bufferViews[normalAccessor.bufferView];
    size_t normalBufferByteStride = 0;
    if (normalBufferView.byteStride == 0) { normalBufferByteStride = sizeof(h_core::math::Vector3); }
    else { normalBufferByteStride = normalBufferView.byteStride; }
    const uint8_t* normalBuffer =
        model.buffers[normalBufferView.buffer].data.data() + normalBufferView.byteOffset + normalAccessor.byteOffset;

    // uv attribute
    uint32_t texCoordAccessorIndex = primitiveInfo.attributes["TEXCOORD_0"];
    tinygltf::Accessor texCoordAccessor = model.accessors[texCoordAccessorIndex];
    tinygltf::BufferView texCoordBufferView = model.bufferViews[texCoordAccessor.bufferView];
    size_t texCoordBufferByteStride = 0;
    if (texCoordBufferView.byteStride == 0) { texCoordBufferByteStride = sizeof(h_core::math::Vector2); }
    else { texCoordBufferByteStride = texCoordBufferView.byteStride; }
    const uint8_t* texCoordBuffer = model.buffers[texCoordBufferView.buffer].data.data() +
                                    texCoordBufferView.byteOffset + texCoordAccessor.byteOffset;

    // load vertex data
    numVertices = posAccessor.count;
    vertices = new h_core::render::Vertex[numVertices] {};

    for (uint32_t vertexIndex = 0; vertexIndex < numVertices; vertexIndex++) {
        h_core::render::Vertex* vertex = &vertices[vertexIndex];
        vertex->position =
            *reinterpret_cast<const h_core::math::Vector3*>(&posBuffer[vertexIndex * posBufferByteStride]);
        vertex->normal =
            *reinterpret_cast<const h_core::math::Vector3*>(&normalBuffer[vertexIndex * normalBufferByteStride]);
        vertex->texCoord =
            *reinterpret_cast<const h_core::math::Vector2*>(&texCoordBuffer[vertexIndex * texCoordBufferByteStride]);
    }

    // load index buffer
    uint32_t indexBufferAccessorIndex = primitiveInfo.indices;
    tinygltf::Accessor indexBufferAccessor = model.accessors[indexBufferAccessorIndex];
    tinygltf::BufferView indexBufferView = model.bufferViews[indexBufferAccessor.bufferView];
    numIndices = indexBufferAccessor.count;

    // determine index type
    size_t meshIndexTypeSize = 0;
    switch (indexBufferAccessor.componentType) {
        case 5121:
            meshIndexType = h_core::render::MeshIndexType::BYTE;
            meshIndexTypeSize = 1;
            break;
        case 5123:
            meshIndexType = h_core::render::MeshIndexType::SHORT;
            meshIndexTypeSize = 2;
            break;
        case 5125:
            meshIndexType = h_core::render::MeshIndexType::INT;
            meshIndexTypeSize = 4;
            break;

        default:
            return MODEL_INIT_FAIL_INVALID_INDEX_DATA_TYPE;
    }

    const void* indicesSrc =
        model.buffers[indexBufferView.buffer].data.data() + indexBufferView.byteOffset + indexBufferAccessor.byteOffset;
    indices = new char[numIndices * meshIndexTypeSize];
    memcpy(indices, indicesSrc, numIndices * meshIndexTypeSize);


    // TODO: load texture data from glb or gltf file, if specified
    if (primitiveInfo.material != -1) {
        tinygltf::Material material = model.materials[primitiveInfo.material];
        HYLOG_INFO("MESH: Loading material %s", material.name.c_str());
        if (material.pbrMetallicRoughness.baseColorTexture.index != -1) {
            tinygltf::Texture gltfTexture = model.textures[material.pbrMetallicRoughness.baseColorTexture.index];
            HYLOG_INFO("MESH: Loading texture %s ?", gltfTexture.name.c_str());

            tinygltf::Sampler sampler = model.samplers[gltfTexture.sampler];
            tinygltf::Image image = model.images[gltfTexture.source];

            GL_WRAP_MODE_TO_WRAP_MODE(sampler.wrapS, wrapU);
            GL_WRAP_MODE_TO_WRAP_MODE(sampler.wrapT, wrapV);

            texture.init(
                image.image.data(), image.image.size(), image.width, image.height, image.component,
                sampler.magFilter == TINYGLTF_TEXTURE_FILTER_NEAREST ? h_core::render::Filter::NEAREST
                                                                     : h_core::render::Filter::LINEAR,
                sampler.minFilter == TINYGLTF_TEXTURE_FILTER_NEAREST ? h_core::render::Filter::NEAREST
                                                                     : h_core::render::Filter::LINEAR,
                wrapU, wrapV);
        }
    }

    return 0;
}

void h_core::render::Mesh::precompile(bool useGles3) {
#ifndef HYCORE_HEADLESS
    // Generate buffers and load attributes
    if (!useGles3) {
        ::glGenVertexArrays(1, &vertexAttributesHandle);
        ::glBindVertexArray(vertexAttributesHandle);
    }
    else { HYLOG_WARN("MESH: Using GLES3"); }

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

    //    // Bind the vertex and index buffers to this VAO
    //    ::glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
    //    ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferHandle);

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

        ::glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, static_cast<uint64_t>(indexTypeSize) * numIndices, indices, GL_STATIC_DRAW);
    }

    // Clean up
    if (!useGles3) { ::glBindVertexArray(0); }
    ::glBindBuffer(GL_ARRAY_BUFFER, 0);
    ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Precompile our texture
    texture.precompile();
#else
    HYLOG_INFO("MESH: Can't precompile (headless)");
#endif
}


void h_core::render::Mesh::addToPacked(uint8_t* _writeHead) {
    uint32_t indexSize = 1;
    switch (meshIndexType) {
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

    uint8_t* writeHead = _writeHead;
    uint8_t* originalHead = _writeHead;

    // # vertices
    memcpy(writeHead, (uint8_t*)&numVertices, sizeof(uint32_t) * 1);
    writeHead += sizeof(uint32_t) * 1;

    // vertices
    memcpy(writeHead, vertices, sizeof(h_core::render::Vertex) * numVertices);
    writeHead += sizeof(h_core::render::Vertex) * numVertices;

    // index size
    memcpy(writeHead, (uint8_t*)&indexSize, sizeof(uint32_t) * 1);
    writeHead += sizeof(uint32_t) * 1;

    // # indices
    memcpy(writeHead, (uint8_t*)&numIndices, sizeof(uint32_t) * 1);
    writeHead += sizeof(uint32_t) * 1;

    // indices
    memcpy(writeHead, indices, indexSize * numIndices);
    writeHead += indexSize * numIndices;

    // texture
    texture.addToPacked(writeHead);
    writeHead += texture.getPackedSize();

    assert(writeHead - originalHead == getPackedSize());
}

size_t h_core::render::Mesh::getPackedSize() const {
    // Try to keep in sync with actual byte requirements
    uint32_t indexSize = 1;
    switch (meshIndexType) {
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

    size_t byteLength = sizeof(uint32_t) * 3 + sizeof(h_core::render::Vertex) * numVertices + indexSize * numIndices +
                        texture.getPackedSize();

    return byteLength;
}

void h_core::render::Mesh::readFromPacked(const uint8_t* _readHead) {
    const uint8_t* readHead = _readHead;

    HYLOG_DEBUG("MESH: loading from packed\n");
    HYLOG_DEBUG("MESH: first byte %x\n", readHead[0]);

    // # vertices
    numVertices = *(uint32_t*)(readHead);
    HYLOG_DEBUG("MESH: # of vertices: %d\n", numVertices);
    readHead += sizeof(uint32_t);

    // vertices data
    vertices = new h_core::render::Vertex[numVertices];
    h_core::render::Vertex* newVerts = (h_core::render::Vertex*)readHead;
    for (uint32_t vertexIndex = 0; vertexIndex < numVertices; vertexIndex++) {
        vertices[vertexIndex] = newVerts[vertexIndex];
    }
    memcpy(
        vertices, readHead, numVertices * sizeof(h_core::render::Vertex));
    readHead += numVertices * sizeof(h_core::render::Vertex);

    // index type
    uint32_t indexTypeSize = *(uint32_t*)(readHead);
    switch (indexTypeSize) {
        case 1:
            meshIndexType = MeshIndexType::BYTE;
            break;
        case 2:
            meshIndexType = MeshIndexType::SHORT;
            break;
        case 4:
            meshIndexType = MeshIndexType::INT;
            break;
        default:
            HYLOG_ERROR("MESH: Invalid packed mesh index type %d", indexTypeSize);
            return;
    }
    readHead += sizeof(uint32_t);

    // # indices
    numIndices = *(uint32_t*)(readHead);
    HYLOG_DEBUG("MESH: # of indices: %d\n", numIndices);
    readHead += sizeof(uint32_t);

    // indices data
    switch (meshIndexType) {
        case MeshIndexType::BYTE:
            indices = new uint8_t[numIndices];
            break;
        case MeshIndexType::SHORT:
            indices = new uint16_t[numIndices];
            break;
        case MeshIndexType::INT:
            indices = new uint32_t[numIndices];
            break;
    }
    memcpy(indices, readHead, numIndices * indexTypeSize);
    readHead += numIndices * indexTypeSize;

    // Texture
    texture.readFromPacked(readHead);
    readHead += texture.getPackedSize();

    // Store data
    primitiveMode = MeshPrimitiveMode::TRIANGLES;  // TODO: make dynamic
}


uint32_t h_core::render::ModelAsset::initFromYaml(const std::string& basePath, const YAML::Node& yaml) {
    h_core::Asset::initFromYaml(basePath, yaml);

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
    size_t textLength;
    const void* text = SDL_LoadFile((basePath + gltfFilePath).c_str(), &textLength);
    if (!gltfBinaryMode) {
        success =
            loader.LoadASCIIFromString(&m_model, &errorText, &warningText, static_cast<const char*>(text), textLength, gltfBasePath);
    }
    else {
        success = loader.LoadBinaryFromMemory(&m_model, &errorText, &warningText, static_cast<const unsigned char*>(text), textLength, gltfBasePath);
    }

    if (!warningText.empty()) { HYLOG_WARN("MODEL: %s\n", warningText.c_str()); }

    if (!success) {
        HYLOG_ERROR("MODEL: %s\n", errorText.c_str());
        HYLOG_ERROR(
            "MODEL: gltfFilePath = %s, gltfBasePath = %s, gltfBinaryMode = %s\n", gltfFilePath.c_str(),
            gltfBasePath.c_str(), gltfBinaryMode ? "YES" : "NO");
        return MODEL_INIT_FAIL_BAD_GLTF;
    }

    for (auto& node : m_model.nodes) {
        if (node.mesh == -1) continue;

        Mesh mesh {};
        mesh.initFromNode(m_model, node);
        m_meshes.push_back(mesh);
    }

    return 0;
}

uint32_t h_core::render::ModelAsset::precompile(h_core::RuntimeSystems* systems) {
#ifndef HYCORE_HEADLESS
    if (m_isCube) {
        // Just load cube
        HYLOG_ERROR("MODEL: primitive cube no longer supported\n");
        // loadModel(8, cubeVertices, 36, cubeTriList, h_core::render::MeshIndexType::SHORT,
        // systems->renderer->isGles3());

        return 0;
    }

    for (uint32_t meshIndex = 0; meshIndex < getMeshCount(); meshIndex++) {
        m_meshes[meshIndex].precompile(systems->renderer->isGles3());

        HYLOG_INFO(
            "MESH: Loaded %zu vertices (%zu indices)\n", m_meshes[meshIndex].numVertices,
            m_meshes[meshIndex].numIndices);
    }
#else
    HYLOG_INFO("MODEL: Can't precompile (headless)");
#endif

    return 0;
}

h_core::render::Mesh* h_core::render::ModelAsset::getMeshes() {
    return m_meshes.data();
}

uint32_t h_core::render::ModelAsset::getMeshCount() {
    return m_meshes.size();
}

size_t h_core::render::ModelAsset::getPackedSize() const {
    size_t size = 0;

    size += sizeof(uint32_t);

    for (const Mesh& mesh : m_meshes) {
        size += mesh.getPackedSize();
    }

    return size;
}

void h_core::render::ModelAsset::toPacked(uint8_t* _writeHead) {
    HYLOG_DEBUG("MODEL: Packing model...");

    uint8_t* writeHead = _writeHead;
    uint8_t* originalHead = _writeHead;

    // mesh count
    uint32_t meshCount = getMeshCount();
    memcpy(writeHead, (uint8_t*)&meshCount, sizeof(uint32_t) * 1);
    writeHead += sizeof(uint32_t) * 1;

    // meshes
    for (Mesh mesh : m_meshes) {
        mesh.addToPacked(writeHead);
        writeHead += mesh.getPackedSize();
    }

    assert(writeHead - originalHead == getPackedSize());
}

void h_core::render::ModelAsset::fromPacked(const uint8_t* _readHead) {
    const uint8_t* readHead = _readHead;
    if(readHead == nullptr) return;

    uint32_t meshCount = *(uint32_t*)readHead;
    readHead += sizeof(uint32_t);

    HYLOG_DEBUG("MODEL: Got %d meshes", meshCount);

    for (uint32_t meshIndex = 0; meshIndex < meshCount; meshIndex++) {
        Mesh mesh {};
        mesh.readFromPacked(readHead);
        m_meshes.push_back(mesh);
        readHead += mesh.getPackedSize();
    }
}

void h_core::render::ModelAsset::doGUI() {
    //    Asset::doGUI();

    uint32_t totalVertices = 0;

    for (Mesh mesh : m_meshes) {
        ImGui::TextColored(IMGUI_COLOR_GOOD, "Name: %s", mesh.name.c_str());
        ImGui::Text("Vertex Count: %d", mesh.numVertices);
        totalVertices += mesh.numVertices;
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

    ImGui::Text("Total Verts: %d", totalVertices);
    ImGui::Text("Total Meshes: %d", getMeshCount());
}
