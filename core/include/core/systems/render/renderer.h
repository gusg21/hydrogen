#pragma once

#include "SDL2/SDL.h"
#include "glad/glad.h"

#include "core/engine.h"
#include "core/systems/render/shader.h"
#include "core/system.h"

namespace h_core {
namespace render {
class Renderer : public System {
  public:
    Renderer() = default;

    uint32_t init(h_core::Engine* engine) override;
    void destroy() override;
    void beginFrame() override;
    void draw() override;
    void endFrame() override;

    uint32_t initFromWindow(
        uint32_t width, uint32_t height, SDL_Window* window);

    SDL_GLContext getGLContext();
    h_core::ComponentBitmask getMask() override;

  private:
    h_core::math::Vector3 m_cameraPosition { 10.f, 10.f, 0.f };
    h_core::math::Vector3 m_cameraDirection { -1.f, 0.f, 0.f };
    float m_fovDegrees = 70.f;
    float m_nearZ = 1.f;
    float m_farZ = 100.f;
    bool m_ccw = true;

    h_core::render::Shader m_shader {};
    SDL_GLContext m_glContext = nullptr;
};
}  // namespace systems
}  // namespace h_core