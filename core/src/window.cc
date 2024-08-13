#include "core/window.h"

#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#define WINDOW_INIT_BGFX_INIT_FAIL 1

uint32_t h_core::Window::init(bx::StringLiteral title,
                              uint32_t          width,
                              uint32_t          height,
                              bool              fullscreen) {
    SDL_Init(0);
    m_sdlWindow =
        SDL_CreateWindow(title.getCPtr(), width, height, SDL_WINDOW_RESIZABLE);

    bgfx::renderFrame();  // TODO: required? Docs say needed to indicate
                          // single-threaded rendering.

    bgfx::Init init;

    // Acquire native handle
#if BX_PLATFORM_WINDOWS
    SDL_PropertiesID props       = SDL_GetWindowProperties(m_sdlWindow);
    void*            win32Handle = SDL_GetPointerProperty(
        props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
    init.platformData.nwh = win32Handle;
#endif

    // Set up resolution + backbuffer settings
    init.resolution.width  = width;
    init.resolution.height = height;
    init.resolution.reset  = BGFX_RESET_VSYNC;

    // Go bgfx, go!
    if (bgfx::init(init) != 0) {
        return WINDOW_INIT_BGFX_INIT_FAIL;
    }

    return 0;
}

void h_core::Window::destroy() {
    SDL_Quit();
}

void h_core::Window::postEventsToQueue(h_core::EventQueue* queue) {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_EVENT_QUIT:
                queue->postEvent(h_core::Event(ENGINE_EVENT_QUIT));
                break;
            case SDL_EVENT_WINDOW_RESIZED: {
                uint32_t      width  = sdlEvent.window.data1;
                uint32_t      height = sdlEvent.window.data2;
                h_core::Event hEvent = h_core::Event(ENGINE_EVENT_RESIZED);
                hEvent.newWidth      = width;
                hEvent.newHeight     = height;
                queue->postEvent(hEvent);
                break;
            }

            default:
                break;
        }
    }
}