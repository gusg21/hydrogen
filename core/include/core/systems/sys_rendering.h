#pragma once

#include "core/system.h"
#include "SDL2/SDL.h"
#include "glad/glad.h"

namespace h_core {
namespace systems {

struct Shader {
    GLuint vertexShader, fragmentShader;
    GLuint program;
};

struct Vertex {
    h_core::math::Vector3 position;
    h_core::math::Vector3 normal;
    h_core::math::Vector2 texCoord;
};

class Rendering : public System {
  public:
    Rendering() = default;

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

    float m_width = 0;
    float m_height = 0;
    Shader m_shader;
    SDL_GLContext m_glContext;
};
}  // namespace systems
}  // namespace h_core