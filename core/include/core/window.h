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

    uint32_t init(
        tinystl::string title, uint32_t width, uint32_t height,
        bool fullscreen);
    void destroy();
    void postEventsToQueue(h_core::EventQueue* queue);

  private:
    SDL_Window* m_sdlWindow = nullptr;
    bgfx::ViewId m_clearViewId = 0;
};
}  // namespace h_core
