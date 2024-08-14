#pragma once

#include <stdint.h>

#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <bx/allocator.h>
#include <tinystl/string.h>

#include "core/engineevents.h"
#include "core/eventqueue.h"

namespace h_core {
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
        tinystl::string title, uint32_t width, uint32_t height,
        bool fullscreen);

    /// @brief clean up the window
    void destroy();

    /// @brief post the window's events to the queue
    /// @param queue a pointer to the queue to populate
    void postEventsToQueue(h_core::EventQueue* queue);

  private:
    SDL_Window* m_sdlWindow = nullptr;
    bgfx::ViewId m_clearViewId = 0;
};
}  // namespace h_core
