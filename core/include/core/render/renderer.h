#pragma once

#ifndef HYCORE_HEADLESS

#include "SDL2/SDL.h"
#include "glad/glad.h"

#include "core/engine.h"
#include "core/input/input.h"
#include "core/runtimesystem.h"
#include "core/transform.h"
#include "meshasset.h"
#include "shader.h"

#define RENDERING_LOAD_SHADER_FAIL_BAD_SHADER_COMPILE  1
#define RENDERING_LOAD_SHADER_FAIL_BAD_FILE_STREAM     2
#define RENDERING_LOAD_PROGRAM_FAIL_BAD_SHADER_COMPILE 1
#define RENDERING_LOAD_PROGRAM_FAIL_BAD_LINK           2
#define RENDERING_INIT_FAIL_BAD_PROGRAM                1
#define RENDERING_OPENGL_LOG_MAX_SIZE                  1024

namespace h_core {
namespace render {
class Renderer : public RuntimeSystem {
  public:
    Renderer() = default;

    uint32_t init(h_core::RuntimeEngine* engine) override;
    void destroy() override;
    void doGUI() override;
    void beginFrame() override;
    void draw() override;
    void endFrame() override;

    [[nodiscard]] SDL_GLContext getGLContext() const;
    [[nodiscard]] bool isGles3();

    static uint32_t loadShader(GLuint* out_shaderId, std::string filePath);
    static uint32_t loadProgram(h_core::render::Shader* out_shader, std::string vertexPath, std::string fragmentPath);

    HYSYSTEM(h_core::Transform::getMask() | h_core::render::MeshComp::getMask());

  protected:
    void setRendererName(const std::string& name);
    void setIsGles3(bool isGles3);

    h_core::math::Vector3 m_cameraPosition { -10.f, 10.f, 0.f };
    h_core::math::Vector3 m_cameraDirection { 1.f, 0.f, 0.f };

    float m_fovDegrees = 70.f;
    float m_nearZ = 1.f;
    float m_farZ = 100.f;
    bool m_ccw = true;

  private:
    static uint32_t callback_setFov(const std::string& args, void* data);

    std::string m_rendererName { "UNKNOWN" };
    bool m_isGles3 = false;

    // Flycam stuff
    bool m_flyCamEnabled = true;
    float m_flyCamSpeed = 20.f;
    float m_flyCamSensitivity = MATH_PI * 100.f; // Using math constants provides an air of legitimacy
    float m_flyCamYaw = 0.f;
    float m_flyCamPitch = 0.f;
    h_core::input::InputActionIndex m_camForwardInputIndex {};
    h_core::input::InputActionIndex m_camRightInputIndex {};
    h_core::input::InputActionIndex m_camGrabMouseInputIndex {};

    SDL_GLContext m_glContext = nullptr;
};
}  // namespace systems
}  // namespace h_core

#endif