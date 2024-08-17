#include "core/window.h"

#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <imgui_impl_sdl3.h>

#define WINDOW_FAIL_INIT_BGFX 1

uint32_t h_core::Window::init(
    std::string title, uint32_t width, uint32_t height, bool fullscreen) {
    // Init SDL
    SDL_Init(0);

    m_sdlWindow =
        SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);


    // Acquire native handle
#if defined(__WIN32__) || defined(_WIN32)
    SDL_PropertiesID props = SDL_GetWindowProperties(m_sdlWindow);
    void* win32Handle = SDL_GetPointerProperty(
        props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);

    m_renderer = new h_core::system::Rendering();
    m_renderer->initFromWindow(width, height, win32Handle);
#else
#error Non-Windows native window acquisition unimplemented
#endif

    // Set up resolution + backbuffer settings
    // Set up imgui
#if defined(__WIN32__) || defined(_WIN32)
    ImGui_ImplSDL3_InitForD3D(m_sdlWindow);
#elif defined(__APPLE__)
    ImGui_ImplSDL3_InitForMetal(m_sdlWindow);
#elif defined(__linux__)
    ImGui_ImplSDL3_InitForOpenGL(m_sdlWindow, nullptr);
#endif  // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX ? BX_PLATFORM_LINUX ?
        // BX_PLATFORM_EMSCRIPTEN

    return 0;
}

void h_core::Window::destroy() {
    ImGui_ImplSDL3_Shutdown();
    SDL_Quit();
}

void h_core::Window::postEventsToQueue(h_core::EventQueue* queue) {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        ImGui_ImplSDL3_ProcessEvent(&sdlEvent);

        switch (sdlEvent.type) {
            case SDL_EVENT_QUIT: {
                h_core::Event hEvent = h_core::Event();
                hEvent.type = ENGINE_EVENT_QUIT;
                queue->postEvent(hEvent);
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED: {
                uint32_t width = sdlEvent.window.data1;
                uint32_t height = sdlEvent.window.data2;
                h_core::Event hEvent = h_core::Event();
                hEvent.type = ENGINE_EVENT_RESIZED;
                hEvent.newWidth = width;
                hEvent.newHeight = height;
                queue->postEvent(hEvent);
                break;
            }

            default:
                break;
        }
    }

    ImGui_ImplSDL3_NewFrame();
}