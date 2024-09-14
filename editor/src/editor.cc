//
// Created by angus.goucher on 9/8/2024.
//

#include "editor/editor.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>
#include <glad/glad.h>
#include <imgui.h>

#include "core/engineevents.h"
#include "core/log.h"
#include "core/theming/theming.h"
#include "editor/windows/meshimporter.h"
#include "editor/windows/projectexplorer.h"

uint32_t h_editor::Editor::init(const h_core::project::Project& project, const std::string& projectBasePath) {
    m_window = new h_core::Window();
    m_window->init("hydrogen editor - " + project.name, 1600, 900, true);

    h_core::theming::editor();

    h_editor::windows::ProjectExplorer* explorer = new h_editor::windows::ProjectExplorer(projectBasePath);
    m_windows.push_back(explorer);

    windows::MeshImporter* imp = new windows::MeshImporter();
    imp->open("assets/camera/AntiqueCamera.gltf");
    m_windows.push_back(imp);

    return 0;
}

void h_editor::Editor::run() {
    bool running = true;
    while (running) {
        m_window->postEventsToQueue(&m_events);

        for (uint32_t eventIndex = 0; eventIndex < m_events.getSize(); eventIndex++) {
            const h_core::Event* event = &m_events.getHeadPointer()[eventIndex];
            switch (event->type) {
                case ENGINE_EVENT_QUIT:
                    running = false;
                    break;

                default: break;
            }
        }

        ::ImGui_ImplOpenGL3_NewFrame();
        ::ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        doGUI();

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ::ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update the window
        m_window->swap();

        // Clear the queue
        m_events.clear();
    }
}

void h_editor::Editor::destroy() {}

void h_editor::Editor::doGUI() {
    ImGui::ShowDemoWindow();

    for (h_editor::EditorWindow* window : m_windows) {
        window->doGUI();
    }
}
