#pragma once

#include "SDL2/SDL.h"
#include "glad/glad.h"

#include "core/systems/renderer/shader.h"
#include "core/system.h"

namespace h_core {
namespace systems {
class Renderer : public System {
  public:
    Renderer() = default;

    uint32_t init();
    void destroy();
    void beginFrame();
    void draw();
    void endFrame();

    uint32_t initFromWindow(
        uint32_t width, uint32_t height, SDL_Window* window);

    SDL_GLContext getGLContext();
    h_core::ComponentBitmask getMask();

  private:
    h_core::math::Vector3 cameraPosition = h_core::math::Vector3(0);
    h_core::math::Vector3 cameraDirection = h_core::math::Vector3(0);

    h_core::systems::Shader m_shader;
    SDL_GLContext m_glContext;
};
}  // namespace systems
}  // namespace h_core