#include "core/systems/render/renderer.h"

#include <fstream>

#include "SDL2/SDL.h"
#include "glad/glad.h"
#include "imgui_impl_sdl2.h"
#include "yaml-cpp/yaml.h"

#include "core/input/dualkeyinputactionsource.h"
#include "core/input/keyinputactionsource.h"
#include "core/log.h"
#include "core/math/mat4x4.h"

uint32_t h_core::render::Renderer::loadShader(GLuint* out_shaderId, std::string filePath) {
    // Load in the code
    size_t shaderCodeLength;
    const char* shaderCodeRaw = (const char*)SDL_LoadFile(filePath.c_str(), &shaderCodeLength);
    HYLOG_DEBUG("RENDERER: Shader Code:\n%s\n", shaderCodeRaw);
    int shaderCodeLengthInt = shaderCodeLength;
    ::glShaderSource(*out_shaderId, 1, (const GLchar**)&shaderCodeRaw, &shaderCodeLengthInt);

    // Compile + verify
    ::glCompileShader(*out_shaderId);
    GLint status;
    ::glGetShaderiv(*out_shaderId, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        char log[RENDERING_OPENGL_LOG_MAX_SIZE] = { 0 };
        ::glGetShaderInfoLog(*out_shaderId, RENDERING_OPENGL_LOG_MAX_SIZE, nullptr, log);
        HYLOG_ERROR("RENDERER: %s\n", log);
        return RENDERING_LOAD_SHADER_FAIL_BAD_SHADER_COMPILE;
    }

    return 0;
}

uint32_t h_core::render::Renderer::loadProgram(
    h_core::render::Shader* out_shader, std::string vertexPath, std::string fragmentPath) {
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
        ::SDL_Log("ERROR: RENDERER: %s\n", log);
        return RENDERING_LOAD_PROGRAM_FAIL_BAD_LINK;
    }

    HYLOG_DEBUG(
        "RENDERER: Shaders (%s, %s) compiled + linked + loaded successfully\n", vertexPath.c_str(),
        fragmentPath.c_str());

    return 0;
}

uint32_t h_core::render::Renderer::callback_setFov(const std::string& args, void* data) {
    h_core::render::Renderer* self = (h_core::render::Renderer*)data;
    RUNTIMECONSOLE_PARSE_ARGS(args, yaml);

    if (!yaml.IsMap()) return 2;
    self->m_fovDegrees = yaml["fov"].as<float>(70.f);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Set FOV to %.2f degrees", self->m_fovDegrees);
    return 0;
}

uint32_t h_core::render::Renderer::init(h_core::RuntimeEngine* engine) {
    h_core::RuntimeSystem::init(engine);

    engine->getConsole()->newCommandWithHelp(
        "setFov", h_core::render::Renderer::callback_setFov, this, "{ fov: [float] } set the renderer's FOV");

    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ::glEnable(GL_DEPTH_TEST);
    ::glDepthFunc(GL_LEQUAL);
    ::glEnable(GL_CULL_FACE);
    ::glCullFace(GL_BACK);

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
    h_core::RuntimeSystem::doGUI();

    if (ImGui::Begin("Renderer Debugger")) {
        ImGui::Text("Renderer Name: %s", m_rendererName.c_str());

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
        h_core::math::Vector3 up = h_core::math::Vector3::normalize(h_core::math::Vector3::cross(right, forward));
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
}

void h_core::render::Renderer::endFrame() {}

SDL_GLContext h_core::render::Renderer::getGLContext() const {
    return m_glContext;
}

void h_core::render::Renderer::setRendererName(const std::string& name) {
    m_rendererName = name;
}

void h_core::render::Renderer::setIsGles3(bool isGles3) {
    m_isGles3 = isGles3;
}

bool h_core::render::Renderer::isGles3() {
    return m_isGles3;
}
