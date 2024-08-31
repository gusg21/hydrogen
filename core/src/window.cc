#include "core/window.h"

#include "SDL2/SDL.h"
#include "imgui_impl_sdl2.h"

#include "core/systems/render/renderer.h"

#define WINDOW_INIT_FAIL_INIT_RENDERER 1

uint32_t h_core::Window::init(
    std::string title, uint32_t width, uint32_t height, bool fullscreen) {
    // Init SDL
    ::SDL_Init(SDL_INIT_EVERYTHING);

    m_sdlWindow = ::SDL_CreateWindow(
        title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,
        height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    m_renderer = new h_core::render::Renderer();
    if (m_renderer->initFromWindow(width, height, m_sdlWindow)) {
        return WINDOW_INIT_FAIL_INIT_RENDERER;
    }

    ::printf("INFO: RENDERING: OpenGL version: %s\n", ::glGetString(GL_VERSION));


    // Set up resolution + backbuffer settings
    // Set up imgui
    // #if defined(__WIN32__) || defined(_WIN32)
    //     ImGui_ImplSDL3_InitForD3D(m_sdlWindow);
    // #elif defined(__APPLE__)
    //     ImGui_ImplSDL3_InitForMetal(m_sdlWindow);
    // #elif defined(__linux__)
    //     ImGui_ImplSDL3_InitForOpenGL(m_sdlWindow, nullptr);
    // #endif  // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX ? BX_PLATFORM_LINUX ?
    //         // BX_PLATFORM_EMSCRIPTEN

    return 0;
}

void h_core::Window::destroy() {
    ::SDL_DestroyWindow(m_sdlWindow);
    ::SDL_Quit();
}

void h_core::Window::postEventsToQueue(h_core::EventQueue* queue) {
    SDL_Event sdlEvent;
    while (::SDL_PollEvent(&sdlEvent)) {
        ::ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

        switch (sdlEvent.type) {
            case SDL_QUIT: {
                h_core::Event hEvent = h_core::Event();
                hEvent.type = ENGINE_EVENT_QUIT;
                queue->postEvent(hEvent);
                break;
            } // QUIT
            case SDL_WINDOWEVENT: {
                switch (sdlEvent.window.event) {
                    case SDL_WINDOWEVENT_RESIZED: {
                        uint32_t width = sdlEvent.window.data1;
                        uint32_t height = sdlEvent.window.data2;
                        h_core::Event hEvent = h_core::Event();
                        hEvent.type = ENGINE_EVENT_RESIZED;
                        hEvent.newWindowWidth = width;
                        hEvent.newWindowHeight = height;
                        queue->postEvent(hEvent);
                        break;
                    }
                    case SDL_WINDOWEVENT_CLOSE: {
                        h_core::Event hEvent = h_core::Event();
                        hEvent.type = ENGINE_EVENT_QUIT;
                        queue->postEvent(hEvent);
                        break;
                    }

                    default:
                        break;
                }
                break;
            } // WINDOWEVENT

            default:
                break;
        } // SDL POLL EVENT
    }
}

void h_core::Window::swap() {
    ::SDL_GL_SwapWindow(m_sdlWindow);
}

h_core::render::Renderer* h_core::Window::getRendererSystem() {
    return m_renderer;
}

SDL_Window* h_core::Window::getSDLWindow() {
    return m_sdlWindow;
}