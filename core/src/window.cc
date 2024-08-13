#include "core/window.h"

h_core::Window::Window(bx::StringLiteral title, uint32_t width, uint32_t height, bool fullscreen) {
    SDL_Init(0);
    m_window = SDL_CreateWindow(title.getCPtr(), width, height, SDL_WINDOW_RESIZABLE);
}

void h_core::Window::postEventsToQueue(h_core::EventQueue* queue) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            queue->postEvent(h_core::Event(ENGINE_EVENT_QUIT));
            break;
        default:
            break;
        }
    }
}