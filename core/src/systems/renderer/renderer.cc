#include "core/systems/renderer/renderer.h"

#include <fstream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "SDL2/SDL.h"
#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "imgui_impl_sdl2.h"

#include "core/engine.h"
#include "core/math/mat4x4.h"
#include "core/mesh.h"

#define SDL_GL_SetShwapInterval SDL_GL_SetSwapInterval

#define RENDERING_LOAD_SHADER_FAIL_BAD_SHADER_COMPILE  1
#define RENDERING_LOAD_SHADER_FAIL_BAD_FILE_STREAM     2
#define RENDERING_LOAD_PROGRAM_FAIL_BAD_SHADER_COMPILE 1
#define RENDERING_LOAD_PROGRAM_FAIL_BAD_LINK           2
#define RENDERING_INIT_FAIL_BAD_PROGRAM                1
#define RENDERING_OPENGL_LOG_MAX_SIZE                  1024

uint32_t loadShader(GLuint* out_shaderId, std::string filePath) {
    // Read code from file
    std::ifstream shaderCodeFile { filePath };
    if (!shaderCodeFile.good()) {
        printf(
            "ERROR: RENDERER: Failed to load shader from path %s\n",
            filePath.c_str());
        return RENDERING_LOAD_SHADER_FAIL_BAD_FILE_STREAM;
    }
    std::stringstream shaderCodeStream;
    shaderCodeStream << shaderCodeFile.rdbuf();
    std::string shaderCode = shaderCodeStream.str();

#if HCORE_DEBUG
    printf("DEBUG: RENDERER: %s\n", shaderCode.c_str());
#endif

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
        printf("ERROR: RENDERER: %s\n", log);
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
        printf("ERROR: RENDERER: %s\n", log);
        return RENDERING_LOAD_PROGRAM_FAIL_BAD_LINK;
    }

#if HCORE_DEBUG
    printf(
        "DEBUG: RENDERER: Shaders (%s, %s) compiled + linked + loaded successfully\n",
        vertexPath.c_str(), fragmentPath.c_str());
#endif

    return 0;
}

uint32_t h_core::systems::Renderer::init() {
    m_shader = h_core::systems::Shader {};
    uint32_t shaderLoadResult = loadProgram(
        &m_shader, "hcore_assets/vs_default.glsl",
        "hcore_assets/fs_default.glsl");
    if (shaderLoadResult != 0) { return RENDERING_INIT_FAIL_BAD_PROGRAM; }

    return 0;
}

void h_core::systems::Renderer::destroy() {
    SDL_GL_DeleteContext(m_glContext);
}

void h_core::systems::Renderer::beginFrame() {
    glViewport(0, 0, engine->getWidth(), engine->getHeight());
    h_core::math::Color clearColor = engine->getClearColor();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.use();
    h_core::math::Vector3 target2 { 0.f, 00.f, 0.f };
    h_core::math::Vector3 position2 { 5.f, 3.f, 0.f };
    h_core::math::Vector3 up { 0.f, 1.f, 0.f };
    h_core::math::Mat4x4 viewMatrix =
        h_core::math::Mat4x4::lookAtMat(position2, target2);
    h_core::math::Mat4x4 projMatrix =
        h_core::math::Mat4x4::getProjMatrix(70.f, 16.f / 9.f, 10.f, 1.f);
    m_shader.setMat4(
        "uni_viewProjectionMatrix",
        h_core::math::Mat4x4::multiply(projMatrix, viewMatrix));
}

void h_core::systems::Renderer::draw() {
    // printf("DEBUG: RENDERER: actor id %d\n", actorId);

    m_shader.setMat4("uni_modelMatrix", transform->getMatrix());

    GLenum glElementType;
    switch (mesh->getMeshIndexType()) {
        case h_core::MeshIndexType::BYTE:
            glElementType = GL_UNSIGNED_BYTE;
            break;
        case h_core::MeshIndexType::SHORT:
            glElementType = GL_UNSIGNED_SHORT;
            break;
        case h_core::MeshIndexType::INT:
            glElementType = GL_UNSIGNED_INT;
            break;
    }

    glBindVertexArray(mesh->getVertexAttributesHandle());
    glDrawElements(
        GL_LINES, mesh->getNumIndices(), glElementType,
        nullptr);
}

uint32_t h_core::systems::Renderer::initFromWindow(
    uint32_t width, uint32_t height, SDL_Window* window) {
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    return 0;
}

void h_core::systems::Renderer::endFrame() {}

h_core::ComponentBitmask h_core::systems::Renderer::getMask() {
    return TRANSFORM_COMPONENT_BITMASK | MODEL_COMPONENT_BITMASK;
}

SDL_GLContext h_core::systems::Renderer::getGLContext() {
    return m_glContext;
}