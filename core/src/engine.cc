#include "core/engine.h"

#include <bgfx/bgfx.h>
#include <imgui.h>
#include <imgui_impl_bgfx.h>
#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#include <tiny_gltf.h>
#include <tinystl/string.h>

#include "core/systems/sys_gravity.h"
#include "core/systems/sys_rendering.h"

void h_core::Engine::init(h_core::Project* project) {
    m_systems[0] = new h_core::systems::Gravity();

    m_project = project;

    ImGui::CreateContext();

    std::string windowTitle = "hydrogen runtime - " + project->projectName;

    m_window = new h_core::Window();
    m_window->init(windowTitle, 1600, 900, false);
    m_systems[1] = m_window->getRenderingSystem();

    m_clearView = 0;
    bgfx::setViewClear(m_clearView, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(m_clearView, 0, 0, bgfx::BackbufferRatio::Equal);

    ImGui_Implbgfx_Init(255);
}

void h_core::Engine::destroy() {
    ImGui_Implbgfx_Shutdown();

    m_window->destroy();
    delete m_window;

    ImGui::DestroyContext();
    bgfx::shutdown();
}

void h_core::Engine::run() {
    // Set up first scene
    if (m_project->initialSceneSpec != ASSETS_ASSET_INDEX_BAD) {
        m_scene.initFromSceneSpecAssetIndex(
            &m_project->assets, m_project->initialSceneSpec);
    }

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
                    bgfx::reset(
                        event.newWidth, event.newHeight, BGFX_RESET_VSYNC);
                    bgfx::setViewRect(
                        m_clearView, 0, 0, bgfx::BackbufferRatio::Equal);
                    break;
                default:
                    break;
            }
        }

        for (uint32_t systemIndex = 0; systemIndex < ENGINE_SYSTEM_COUNT;
             systemIndex++) {
            m_scene.processSystem(m_systems[systemIndex]);
        }

        ImGui_Implbgfx_NewFrame();

        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());

        // BEGIN PLACEHOLDER RENDERING PLAYGROUND //

        bgfx::touch(m_clearView);
        bgfx::setDebug(BGFX_DEBUG_STATS);

        for (uint32_t systemIndex = 0; systemIndex < ENGINE_SYSTEM_COUNT;
             systemIndex++) {
            m_scene.drawSystem(m_systems[systemIndex]);
        }

        // END PLACEHOLDER RENDERING PLAYGROUND //

        for (uint32_t systemIndex = 0; systemIndex < ENGINE_SYSTEM_COUNT;
             systemIndex++) {
            m_systems[systemIndex]->endFrame();
        }

        m_events.clear();
    }

    bgfx::shutdown();
}
