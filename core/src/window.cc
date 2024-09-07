#include "core/window.h"

#include "SDL2/SDL.h"
#include "imgui_impl_sdl2.h"

#include "core/systems/render/renderer.h"

#define SDL_GL_SetShwapInterval SDL_GL_SetSwapInterval

#define WINDOW_INIT_FAIL_INIT_RENDERER 1

uint32_t h_core::Window::init(std::string title, uint32_t width, uint32_t height, bool fullscreen) {
    // Init SDL
    ::SDL_Init(SDL_INIT_EVERYTHING);

//    ::SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_);
#if __ANDROID__
    ::SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    ::SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    ::SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    m_isGles3 = true;
#else
    ::SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    ::SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    ::SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    m_isGles3 = false;
#endif
    ::SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    ::SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    ::SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    m_sdlWindow = ::SDL_CreateWindow(
        title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    m_glContext = ::SDL_GL_CreateContext(m_sdlWindow);
    if (m_glContext == nullptr) {
        ::SDL_Log("ERROR: WINDOW: GL Context error: %s", SDL_GetError());
        return 1; // TODO: Make actual error code
    }
    ::SDL_GL_MakeCurrent(m_sdlWindow, m_glContext);

    ::SDL_GL_SetShwapInterval(0);

    int gladInitResult = ::gladLoadGLLoader(::SDL_GL_GetProcAddress);
    if (gladInitResult == 0) {
        ::SDL_Log("ERROR: WINDOW: Failed to init OpenGL context\n");
        return 1; // TODO: Make actual error code
    }
    const uint8_t* glVersionStr = ::glGetString(GL_VERSION);
    if (glVersionStr == nullptr) { // TODO: does the above if block cover all cases?
        ::SDL_Log("ERROR: WINDOW: OpenGL version error: %u\n", glGetError());
        return 1; // TODO: Make actual error code
    }
    ::SDL_Log("INFO: WINDOW: OpenGL version: %s\n", glVersionStr);

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
    ::SDL_GL_DeleteContext(m_glContext);
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
            }  // QUIT
            case SDL_MOUSEMOTION: {
                h_core::Event hEvent = h_core::Event();
                hEvent.type = ENGINE_EVENT_MOUSE_MOTION;
                hEvent.mouseDx = sdlEvent.motion.xrel;
                hEvent.mouseDy = sdlEvent.motion.yrel;
                hEvent.mouseX = sdlEvent.motion.x;
                hEvent.mouseY = sdlEvent.motion.y;
                queue->postEvent(hEvent);
                break;
            }
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
            }  // WINDOWEVENT

            default:
                break;
        }  // SDL POLL EVENT
    }
}

void h_core::Window::swap() {
    ::SDL_GL_SwapWindow(m_sdlWindow);
}

SDL_Window* h_core::Window::getSDLWindow() const {
    return m_sdlWindow;
}

SDL_GLContext h_core::Window::getGLContext() const {
    return m_glContext;
}
bool h_core::Window::isGles3() const {
    return false;
}
