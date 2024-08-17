#include "core/systems/sys_rendering.h"

#include "SDL3/SDL.h"
#include "bgfx/bgfx.h"
#include "bgfx/entry/entry.h"
#include "bgfx/platform.h"
#include "imgui_impl_sdl3.h"

#include "core/math/mat4x4.h"
#include "core/model.h"

#define RENDERING_INIT_FAIL_UNABLE_TO_BGFX    1
#define RENDERING_MEM_LOAD_FAILED_UNABLE_OPEN 1

bgfx::ShaderHandle loadShader(const bx::StringView& shaderName) {
    bx::FilePath filePath("shaders/");

    switch (bgfx::getRendererType()) {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12:
            filePath.join("dx11");
            break;
        case bgfx::RendererType::Agc:
        case bgfx::RendererType::Gnm:
            filePath.join("pssl");
            break;
        case bgfx::RendererType::Metal:
            filePath.join("metal");
            break;
        case bgfx::RendererType::Nvn:
            filePath.join("nvn");
            break;
        case bgfx::RendererType::OpenGL:
            filePath.join("glsl");
            break;
        case bgfx::RendererType::OpenGLES:
            filePath.join("essl");
            break;
        case bgfx::RendererType::Vulkan:
            filePath.join("spirv");
            break;

        case bgfx::RendererType::Count:
            BX_ASSERT(false, "You should not be here!");
            break;
    }

    char fileName[512];
    bx::strCopy(fileName, BX_COUNTOF(fileName), shaderName);
    bx::strCat(fileName, BX_COUNTOF(fileName), ".bin");

    filePath.join(fileName);

    handle = bgfx::createShader(bgfx::makeRef(fileName, sizeof(fileName)));

    return handle;
}

bgfx::ProgramHandle loadProgram(
    const bx::StringView& vsName, const bx::StringView& fsName) {
    return bgfx::createProgram(loadShader(vsName), loadShader(fsName));
}

uint32_t h_core::system::Rendering::init() {
    return 0;
}

uint32_t h_core::system::Rendering::initWindow(
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

    m_program = loadProgram("vs_default", "fs_default");

    return 0;
}


void h_core::system::Rendering::process() {
    h_core::math::Mat4x4 view = h_core::math::Mat4x4::lookAtMat(
        cameraPosition,
        h_core::math::Vector3::add(cameraPosition, cameraDirection), true);

    h_core::math::Mat4x4 proj = h_core::math::Mat4x4::getProjMatrix(
        60.0f, m_width / m_height, 0.1f, 100.0f,
        bgfx::getCaps()->homogeneousDepth, true);

    bgfx::setViewTransform(0, view.matrix, proj.matrix);

    bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

    
    bgfx::setTransform(transform->getMatrix());
    
    bgfx::setVertexBuffer(0, model->getVertexBuffer());
    bgfx::setIndexBuffer(model->getIndexBuffer());
    
    uint64_t state = 0 | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_WRITE_Z |
                     BGFX_STATE_WRITE_RGB;
    bgfx::setState(state);

    bgfx::submit(0, m_program);
}


h_core::ComponentBitmask h_core::system::Rendering::getMask() {
    return TRANSFORM_COMPONENT_BITMASK & MODEL_COMPONENT_BITMASK;
}