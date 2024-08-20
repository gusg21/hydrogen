#include "core/systems/sys_rendering.h"

#include <fstream>

#include "SDL2/SDL.h"
#include "glad/glad.h"
#include "imgui_impl_sdl2.h"

#include "core/engine.h"
#include "core/math/mat4x4.h"
#include "core/mesh.h"

#define SDL_GL_SetShwapInterval SDL_GL_SetSwapInterval

#define RENDERING_LOAD_SHADER_FAIL_BAD_SHADER_COMPILE  1
#define RENDERING_LOAD_PROGRAM_FAIL_BAD_SHADER_COMPILE 1
#define RENDERING_LOAD_PROGRAM_FAIL_BAD_LINK           2
#define RENDERING_INIT_FAIL_BAD_PROGRAM                1
#define RENDERING_OPENGL_LOG_MAX_SIZE                  1024

// uint32_t loadShader(
//     bgfx::ShaderHandle* out_handle, std::string* out_codeString,
//     std::string shaderName) {
//     std::string shaderPath = "generated/";

// #if defined(_WIN32)
//     shaderPath += "windows/";
// #elif defined(__APPLE__)
//     shaderPath += "osx/";
// #endif

//     shaderPath += shaderName + ".bin";

//     std::ifstream shaderCodeFile { shaderPath };

//     if (!shaderCodeFile.good()) {
//         printf(
//             "ERROR: RENDERING: Failed to open shader file %s\n",
//             shaderPath.c_str());
//         return 1;
//     }

//     printf("INFO: RENDERING: Loaded shader file %s\n", shaderPath.c_str());


//     *out_codeString = shaderCodeStream.str();

//     *out_handle = bgfx::createShader(
//         bgfx::makeRef(out_codeString->c_str(), out_codeString->size()));

//     return 0;
// }

uint32_t loadShader(GLuint* out_shaderId, std::string filePath) {
    // Read code from file
    std::ifstream shaderCodeFile { filePath };
    std::stringstream shaderCodeStream;
    shaderCodeStream << shaderCodeFile.rdbuf();
    std::string shaderCode = shaderCodeStream.str();

    // Load in the code
    const GLint shaderCodeLength = shaderCode.size();
    const char* shaderCodeRaw = shaderCode.data();
    glShaderSource(
        *out_shaderId, 1, (const GLchar**)&shaderCodeRaw, &shaderCodeLength);

    // Compile + verify
    glCompileShader(*out_shaderId);
    GLint status;
    glGetShaderiv(*out_shaderId, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        char log[RENDERING_OPENGL_LOG_MAX_SIZE] = { 0 };
        glGetShaderInfoLog(
            *out_shaderId, RENDERING_OPENGL_LOG_MAX_SIZE, nullptr, log);
        printf("ERROR: RENDERING: %s\n", log);
        return RENDERING_LOAD_SHADER_FAIL_BAD_SHADER_COMPILE;
    }

    return 0;
}

uint32_t loadProgram(
    h_core::systems::Shader* out_shader, std::string vertexPath,
    std::string fragmentPath) {
    uint32_t result;

    out_shader->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    result = loadShader(&out_shader->vertexShader, vertexPath);
    if (result != 0) { return RENDERING_LOAD_PROGRAM_FAIL_BAD_SHADER_COMPILE; }

    out_shader->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    result = loadShader(&out_shader->fragmentShader, fragmentPath);
    if (result != 0) { return RENDERING_LOAD_PROGRAM_FAIL_BAD_SHADER_COMPILE; }

    out_shader->program = glCreateProgram();
    glAttachShader(out_shader->program, out_shader->vertexShader);
    glAttachShader(out_shader->program, out_shader->fragmentShader);

    glBindAttribLocation(out_shader->program, 0, "in_position");
    glBindAttribLocation(out_shader->program, 1, "in_normal");
    glBindAttribLocation(out_shader->program, 2, "in_texCoord");
    glLinkProgram(out_shader->program);

    GLint linkStatus;
    glGetProgramiv(out_shader->program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        char log[RENDERING_OPENGL_LOG_MAX_SIZE] = { 0 };
        glGetProgramInfoLog(
            out_shader->program, RENDERING_OPENGL_LOG_MAX_SIZE, nullptr, log);
        printf("ERROR: RENDERING: %s\n", log);
        return RENDERING_LOAD_PROGRAM_FAIL_BAD_LINK;
    }

#if HCORE_DEBUG
    printf(
        "DEBUG: RENDERER: Shaders (%s, %s) compiled + linked + loaded successfully\n",
        vertexPath.c_str(), fragmentPath.c_str());
#endif

    return 0;
}

uint32_t h_core::systems::Rendering::init() {
    m_shader = h_core::systems::Shader {};
    uint32_t shaderLoadResult = loadProgram(
        &m_shader, "hcore_assets/vs_default.glsl",
        "hcore_assets/fs_default.glsl");
    if (shaderLoadResult != 0) { return RENDERING_INIT_FAIL_BAD_PROGRAM; }

    return 0;
}

void h_core::systems::Rendering::destroy() {
    SDL_GL_DeleteContext(m_glContext);
}

void h_core::systems::Rendering::beginFrame() {
    glViewport(0, 0, engine->getWidth(), engine->getHeight());
    h_core::math::Color clearColor = engine->getClearColor();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void h_core::systems::Rendering::draw() {
    // h_core::math::Mat4x4 view = h_core::math::Mat4x4::lookAtMat(
    //     cameraPosition,
    //     h_core::math::Vector3::add(cameraPosition, cameraDirection), true);

    // h_core::math::Mat4x4 proj = h_core::math::Mat4x4::getProjMatrix(
    //     60.0f, m_width / m_height, 0.1f, 100.0f,
    //     bgfx::getCaps()->homogeneousDepth, true);

    // bgfx::setViewTransform(0, view.m_matrix, proj.m_matrix);

    // bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));


    // bgfx::setTransform(transform->getMatrix().m_matrix);

    // bgfx::setVertexBuffer(0, model->getVertexBuffer());
    // bgfx::setIndexBuffer(model->getIndexBuffer());

    // uint64_t state = 0 | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_WRITE_Z |
    //                  BGFX_STATE_WRITE_RGB;
    // bgfx::setState(state);

    // bgfx::submit(0, m_shader.programHandle);
}

uint32_t h_core::systems::Rendering::initFromWindow(
    uint32_t width, uint32_t height, SDL_Window* window) {
    // bgfx::renderFrame();  // TODO: required? Docs say needed to indicate
    //                       // single-threaded rendering.

    // bgfx::Init init;

    // // Acquire native handle
    // init.platformData.nwh = nwh;
    // init.debug = true;

    // // Set up resolution + backbuffer settings
    // init.resolution.width = width;
    // init.resolution.height = height;
    // init.resolution.reset = BGFX_RESET_VSYNC;

    // if (!bgfx::init(init)) { return RENDERING_INIT_FAIL_UNABLE_TO_BGFX; }

    // m_width = width;
    // m_height = height;


    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    m_glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, m_glContext);

    SDL_GL_SetShwapInterval(1);

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    return 0;
}

void h_core::systems::Rendering::endFrame() {}

h_core::ComponentBitmask h_core::systems::Rendering::getMask() {
    return TRANSFORM_COMPONENT_BITMASK & MODEL_COMPONENT_BITMASK;
}

SDL_GLContext h_core::systems::Rendering::getGLContext() {
    return m_glContext;
}