#include "core/systems/sys_rendering.h"

#include <fstream>

#include "SDL3/SDL.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "imgui_impl_sdl3.h"

#include "core/math/mat4x4.h"
#include "core/model.h"

#define RENDERING_INIT_FAIL_UNABLE_TO_BGFX    1
#define RENDERING_MEM_LOAD_FAILED_UNABLE_OPEN 1

uint32_t loadShader(
    bgfx::ShaderHandle* out_handle, std::string* out_codeString,
    std::string shaderName) {
    std::string shaderPath = "generated/";

#if defined(_WIN32)
    shaderPath += "windows/";
#elif defined(__APPLE__)
    shaderPath += "osx/";
#endif

    shaderPath += shaderName + ".bin";

    std::ifstream shaderCodeFile { shaderPath };

    if (!shaderCodeFile.good()) {
        printf(
            "ERROR: RENDERING: Failed to open shader file %s\n",
            shaderPath.c_str());
        return 1;
    }

    printf("INFO: RENDERING: Loaded shader file %s\n", shaderPath.c_str());

    std::stringstream shaderCodeStream {};
    shaderCodeStream << shaderCodeFile.rdbuf();
    *out_codeString = shaderCodeStream.str();

    *out_handle = bgfx::createShader(
        bgfx::makeRef(out_codeString->c_str(), out_codeString->size()));

    return 0;
}

void loadProgram(
    h_core::systems::Shader* out_shader, std::string vertexName,
    std::string fragmentName) {
    loadShader(&out_shader->vertexHandle, &out_shader->vertexCode, vertexName);
    loadShader(
        &out_shader->fragmentHandle, &out_shader->fragmentCode, fragmentName);
    out_shader->programHandle = bgfx::createProgram(
        out_shader->vertexHandle, out_shader->fragmentHandle);
}

uint32_t h_core::systems::Rendering::init() {
    return 0;
}

uint32_t h_core::systems::Rendering::initFromWindow(
    uint32_t width, uint32_t height, void* nwh) {
    bgfx::renderFrame();  // TODO: required? Docs say needed to indicate
                          // single-threaded rendering.

    bgfx::Init init;

    // Acquire native handle
    init.platformData.nwh = nwh;

    // Set up resolution + backbuffer settings
    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;

    if (!bgfx::init(init)) { return RENDERING_INIT_FAIL_UNABLE_TO_BGFX; }

    m_width = width;
    m_height = height;

    loadProgram(&m_shader, "vs_default", "fs_default");

    return 0;
}


void h_core::systems::Rendering::process() {}

void h_core::systems::Rendering::draw() {
    printf("BANG\n");

    h_core::math::Mat4x4 view = h_core::math::Mat4x4::lookAtMat(
        cameraPosition,
        h_core::math::Vector3::add(cameraPosition, cameraDirection), true);

    h_core::math::Mat4x4 proj = h_core::math::Mat4x4::getProjMatrix(
        60.0f, m_width / m_height, 0.1f, 100.0f,
        bgfx::getCaps()->homogeneousDepth, true);

    bgfx::setViewTransform(0, view.m_matrix, proj.m_matrix);

    bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));


    bgfx::setTransform(transform->getMatrix().m_matrix);

    bgfx::setVertexBuffer(0, model->getVertexBuffer());
    bgfx::setIndexBuffer(model->getIndexBuffer());

    uint64_t state = 0 | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_WRITE_Z |
                     BGFX_STATE_WRITE_RGB;
    bgfx::setState(state);

    bgfx::submit(0, m_shader.programHandle);
}

void h_core::systems::Rendering::endFrame() {
    bgfx::frame();
}

h_core::ComponentBitmask h_core::systems::Rendering::getMask() {
    return TRANSFORM_COMPONENT_BITMASK & MODEL_COMPONENT_BITMASK;
}