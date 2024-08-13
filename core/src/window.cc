#include "core/window.h"

#include <stdint.h>
#include <SDL3/SDL.h>

h_core::Window::Window(const char* title, uint32_t width, uint32_t height, bool fullscreen) {
    SDL_Init(0);
    m_window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
}