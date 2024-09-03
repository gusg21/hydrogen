#pragma once

#include <string>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "core/engineevents.h"
#include "core/eventqueue.h"

namespace h_core {
namespace render {
class Renderer;
}

class Window {
  public:
    Window() = default;
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    /// @brief set up a new window
    /// @param title the window's title
    /// @param width the width of the window
    /// @param height the height of the window
    /// @param fullscreen fullscreen or no
    /// @return 0 on success, WINDOW_FAIL_* on fail
    uint32_t init(
        std::string title, uint32_t width, uint32_t height, bool fullscreen);

    /// @brief clean up the window
    void destroy();

    /// @brief post the window's events to the queue
    /// @param queue a pointer to the queue to populate
    void postEventsToQueue(h_core::EventQueue* queue);

    void swap();

    [[nodiscard]] SDL_Window* getSDLWindow() const;
    [[nodiscard]] SDL_GLContext getGLContext() const;

  private:
    SDL_GLContext m_glContext = nullptr;
    SDL_Window* m_sdlWindow = nullptr;
};
}  // namespace h_core
