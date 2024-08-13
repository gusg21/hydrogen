#include "core/engine.h"

#include <bgfx/bgfx.h>
#include <imgui.h>
#include <imgui_impl_bgfx.h>

h_core::Engine::Engine() {}

h_core::Engine::~Engine() {
    delete m_window;
}

void h_core::Engine::init(h_core::Project project) {
    m_project = project;

    ImGui::CreateContext();

#define ENGINE_WINDOW_TITLE_MAX_LENGTH 1024
    char windowTitle[ENGINE_WINDOW_TITLE_MAX_LENGTH] = "hydrogen runtime - ";
    bx::strCat(windowTitle, ENGINE_WINDOW_TITLE_MAX_LENGTH,
               project.projectName);
    m_window = new h_core::Window();
    m_window->init(windowTitle, 1600, 900, false);

    m_clearView = 0;
    bgfx::setViewClear(m_clearView, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(m_clearView, 0, 0, bgfx::BackbufferRatio::Equal);

    ImGui_Implbgfx_Init(255);
}

void h_core::Engine::destroy() {
    ImGui_Implbgfx_Shutdown();

    m_window->destroy();

    ImGui::DestroyContext();
    bgfx::shutdown();
}

void h_core::Engine::run() {
    bool engineRunning = true;
    while (engineRunning) {
        m_window->postEventsToQueue(&m_events);

        for (uint32_t eventIndex = 0; eventIndex < m_events.getSize();
             eventIndex++) {
            h_core::Event event = m_events.getHeadPointer()[eventIndex];
            switch (event.type) {
                case ENGINE_EVENT_QUIT:
                    engineRunning = false;
                    break;
                case ENGINE_EVENT_RESIZED:
                    bgfx::reset(event.newWidth, event.newHeight,
                                BGFX_RESET_VSYNC);
                    bgfx::setViewRect(m_clearView, 0, 0,
                                      bgfx::BackbufferRatio::Equal);
                    break;
                default:
                    break;
            }
        }

        ImGui_Implbgfx_NewFrame();

        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());

        // BEGIN PLACEHOLDER RENDERING PLAYGROUND //

        bgfx::touch(m_clearView);
        bgfx::setDebug(BGFX_DEBUG_STATS);
        bgfx::frame();

        // END PLACEHOLDER RENDERING PLAYGROUND //

        m_events.clear();
    }

    bgfx::shutdown();
}
