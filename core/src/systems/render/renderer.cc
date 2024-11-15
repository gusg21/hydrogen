#include "core/systems/render/renderer.h"

#include <fstream>

#include "SDL2/SDL.h"
#include "glad/glad.h"
#include "imgui_impl_sdl2.h"

#include "core/engine.h"
#include "core/input/dualkeyinputactionsource.h"
#include "core/input/keyinputactionsource.h"
#include "core/math/mat4x4.h"
#include "core/systems/render/meshasset.h"


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
        ::printf("ERROR: RENDERER: Failed to load shader from path %s\n", filePath.c_str());
        return RENDERING_LOAD_SHADER_FAIL_BAD_FILE_STREAM;
    }
    std::stringstream shaderCodeStream;
    shaderCodeStream << shaderCodeFile.rdbuf();
    std::string shaderCode = shaderCodeStream.str();

#if HCORE_DEBUG
    // printf("DEBUG: RENDERER: %s\n", shaderCode.c_str());
#endif

    // Load in the code
    const GLint shaderCodeLength = shaderCode.size();
    const char* shaderCodeRaw = shaderCode.data();
    ::glShaderSource(*out_shaderId, 1, (const GLchar**)&shaderCodeRaw, &shaderCodeLength);

    // Compile + verify
    ::glCompileShader(*out_shaderId);
    GLint status;
    ::glGetShaderiv(*out_shaderId, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        char log[RENDERING_OPENGL_LOG_MAX_SIZE] = { 0 };
        ::glGetShaderInfoLog(*out_shaderId, RENDERING_OPENGL_LOG_MAX_SIZE, nullptr, log);
        ::printf("ERROR: RENDERER: %s\n", log);
        return RENDERING_LOAD_SHADER_FAIL_BAD_SHADER_COMPILE;
    }

    return 0;
}

uint32_t loadProgram(h_core::render::Shader* out_shader, std::string vertexPath, std::string fragmentPath) {
    uint32_t result;

    out_shader->vertexShader = ::glCreateShader(GL_VERTEX_SHADER);
    result = loadShader(&out_shader->vertexShader, vertexPath);
    if (result != 0) { return RENDERING_LOAD_PROGRAM_FAIL_BAD_SHADER_COMPILE; }

    out_shader->fragmentShader = ::glCreateShader(GL_FRAGMENT_SHADER);
    result = loadShader(&out_shader->fragmentShader, fragmentPath);
    if (result != 0) { return RENDERING_LOAD_PROGRAM_FAIL_BAD_SHADER_COMPILE; }

    out_shader->program = ::glCreateProgram();
    ::glAttachShader(out_shader->program, out_shader->vertexShader);
    ::glAttachShader(out_shader->program, out_shader->fragmentShader);

    ::glBindAttribLocation(out_shader->program, 0, "in_position");
    ::glBindAttribLocation(out_shader->program, 1, "in_normal");
    ::glBindAttribLocation(out_shader->program, 2, "in_texCoord");
    ::glLinkProgram(out_shader->program);

    GLint linkStatus;
    ::glGetProgramiv(out_shader->program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        char log[RENDERING_OPENGL_LOG_MAX_SIZE] = { 0 };
        ::glGetProgramInfoLog(out_shader->program, RENDERING_OPENGL_LOG_MAX_SIZE, nullptr, log);
        ::printf("ERROR: RENDERER: %s\n", log);
        return RENDERING_LOAD_PROGRAM_FAIL_BAD_LINK;
    }

#if HCORE_DEBUG
    ::printf(
        "DEBUG: RENDERER: Shaders (%s, %s) compiled + linked + loaded successfully\n", vertexPath.c_str(),
        fragmentPath.c_str());
#endif

    return 0;
}

uint32_t h_core::render::Renderer::init(h_core::Engine* engine) {
    h_core::System::init(engine);

    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ::glEnable(GL_DEPTH_TEST);
    ::glDepthFunc(GL_LEQUAL);
    ::glEnable(GL_CULL_FACE);
    ::glCullFace(GL_BACK);

    m_shader = h_core::render::Shader {};
    uint32_t shaderLoadResult = loadProgram(&m_shader, "hcore_assets/vs_default.glsl", "hcore_assets/fs_default.glsl");
    if (shaderLoadResult != 0) { return RENDERING_INIT_FAIL_BAD_PROGRAM; }

    m_camForwardInputIndex = engine->getInput()->newAction("cam_forward");
    engine->getInput()
        ->getAction(m_camForwardInputIndex)
        ->sources.push_back(new h_core::input::DualKeyInputActionSource(SDL_SCANCODE_W, SDL_SCANCODE_S));

    m_camRightInputIndex = engine->getInput()->newAction("cam_right");
    engine->getInput()
        ->getAction(m_camRightInputIndex)
        ->sources.push_back(new h_core::input::DualKeyInputActionSource(SDL_SCANCODE_D, SDL_SCANCODE_A));

    m_camGrabMouseInputIndex = engine->getInput()->newAction("grab_mouse");
    engine->getInput()
        ->getAction(m_camGrabMouseInputIndex)
        ->sources.push_back(new h_core::input::KeyInputActionSource(SDL_SCANCODE_GRAVE));

    m_flyCamYaw = ((::acosf(-m_cameraDirection.z)) / (float)MATH_TAU) * 360.f;

    return 0;
}

void h_core::render::Renderer::destroy() {}

void h_core::render::Renderer::doGUI() {
    h_core::System::doGUI();

    if (ImGui::Begin("Renderer Debugger")) {
        ImGui::SeparatorText("Camera");
        ImGui::DragFloat3("Position", &m_cameraPosition.x);
        ImGui::DragFloat3("Direction", &m_cameraDirection.x);
        ImGui::SliderFloat("FOV", &m_fovDegrees, 0.f, 180.f);
        ImGui::SliderFloat("Near Z", &m_nearZ, 0.001f, 100.f);
        ImGui::SliderFloat("Far Z", &m_farZ, 0.001f, 100.f);

        ImGui::SeparatorText("FlyCam");
        ImGui::Checkbox("Enabled", &m_flyCamEnabled);
        ImGui::SliderFloat("Speed", &m_flyCamSpeed, 0.1f, 100.f);
        ImGui::DragFloat("Sensitivity", &m_flyCamSensitivity, 10.f);
        ImGui::Text("Yaw: %.3f Pitch: %.3f", m_flyCamYaw, m_flyCamPitch);

        ImGui::SeparatorText("Render Configuration");
        ImGui::Checkbox("CCW", &m_ccw);
    }
    ImGui::End();
}

void h_core::render::Renderer::beginFrame() {
    ::glViewport(0, 0, engine->getWidth(), engine->getHeight());
    h_core::math::Color clearColor = engine->getClearColor();
    ::glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ::glFrontFace(m_ccw ? GL_CCW : GL_CW);

    m_shader.use();
    h_core::math::Mat4x4 viewMatrix = h_core::math::Mat4x4::lookAtMat(
        m_cameraPosition, h_core::math::Vector3::add(m_cameraPosition, m_cameraDirection));

    float aspect = (float)engine->getWidth() / (float)engine->getHeight();
    h_core::math::Mat4x4 projMatrix =
        h_core::math::Mat4x4::getProjMatrix((m_fovDegrees / 180.f) * (float)M_PI, aspect, m_nearZ, m_farZ);
    m_shader.setMat4("uni_viewProjectionMatrix", h_core::math::Mat4x4::multiply(projMatrix, viewMatrix));

    // Camera controls
    if (m_flyCamEnabled) {
        // Mouse looking (if captured)
        if (engine->getInput()->mouseCaptured) {
            m_flyCamYaw += engine->getInput()->getMouseDeltaX() * m_flyCamSensitivity * engine->getDeltaSecs();
            m_flyCamPitch -= engine->getInput()->getMouseDeltaY() * m_flyCamSensitivity * engine->getDeltaSecs();
            m_flyCamPitch = MATH_CLAMP(m_flyCamPitch, -89.0f, 89.0f);
        }

        // Calculate camera orientation space
        float yawRad = (m_flyCamYaw / 360.f) * MATH_TAU;
        float pitchRad = (m_flyCamPitch / 360.f) * MATH_TAU;
        h_core::math::Vector3 forward {};
        forward.x = ::cosf(pitchRad) * ::sinf(yawRad);
        forward.y = ::sinf(pitchRad);
        forward.z = ::cosf(pitchRad) * -::cosf(yawRad);
        forward = h_core::math::Vector3::normalize(forward);
        h_core::math::Vector3 right = h_core::math::Vector3::normalize(
            h_core::math::Vector3::cross(forward, h_core::math::Vector3 { 0.f, 1.f, 0.f }));
        h_core::math::Vector3 up = h_core::math::Vector3::normalize(
            h_core::math::Vector3::cross(right, forward));
        m_cameraDirection = forward;

        // Move along axes in cam orientation space
        float forwardAmount = engine->getInput()->getAnalogValue(m_camForwardInputIndex);
        m_cameraPosition += forward * forwardAmount * engine->getDeltaSecs() * m_flyCamSpeed;

        float rightAmount = engine->getInput()->getAnalogValue(m_camRightInputIndex);
        m_cameraPosition += right * rightAmount * engine->getDeltaSecs() * m_flyCamSpeed;

        // Toggle mouse capture
        if (engine->getInput()->getDigitalPressed(m_camGrabMouseInputIndex)) {
            engine->getInput()->mouseCaptured = !engine->getInput()->mouseCaptured;
        }
    }
}

void h_core::render::Renderer::draw() {
    // printf("DEBUG: RENDERER: actor id %d\n", actorId);

    m_shader.setMat4("uni_modelMatrix", transform->getMatrix());

    GLenum glElementType;
    switch (meshComp->mesh->getMeshIndexType()) {
        case h_core::render::MeshIndexType::BYTE:
            glElementType = GL_UNSIGNED_BYTE;
            break;
        case h_core::render::MeshIndexType::SHORT:
            glElementType = GL_UNSIGNED_SHORT;
            break;
        case h_core::render::MeshIndexType::INT:
            glElementType = GL_UNSIGNED_INT;
            break;
    }

    ::glBindVertexArray(meshComp->mesh->getVertexAttributesHandle());
    ::glDrawElements(meshComp->mesh->getPrimitiveMode(), meshComp->mesh->getNumIndices(), glElementType, nullptr);
}

void h_core::render::Renderer::endFrame() {}

h_core::ComponentBitmask h_core::render::Renderer::getMask() const {
    return TRANSFORM_COMPONENT_BITMASK | MODEL_COMPONENT_BITMASK;
}

SDL_GLContext h_core::render::Renderer::getGLContext() const {
    return m_glContext;
}
