#include "core/model.h"

#include <string>

#include "tiny_gltf.h"

// TODO: move these godless monstrosities to a subf

static h_core::VertexData cubeVertices[] = {
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


uint32_t h_core::Model::initFromYaml(h_core::Assets* assets, YAML::Node yaml) {
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
    const uint8_t* buffer = model.buffers.front().data.data();
    

    // Create data buffers
    m_vertexBuffer = bgfx::createVertexBuffer(
        bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), VertexData::layout);

    m_indexBuffer = bgfx::createIndexBuffer(
        bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));

    return 0;
}

void h_core::VertexData::init() {
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float, true);
}