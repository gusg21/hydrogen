#pragma once

#include <stdint.h>

#include <SDL3/SDL.h>
#include <bx/string.h>

#include "core/engineevents.h"
#include "core/eventqueue.h"

namespace h_core {
class Window {
public:
    Window(bx::StringLiteral title,
           uint32_t      width,
           uint32_t      height,
           bool          fullscreen);
    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

    void postEventsToQueue(h_core::EventQueue* queue);

private:
    SDL_Window* m_window = nullptr;
};
}  // namespace h_core
