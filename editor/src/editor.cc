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
#include "editor/asseteditorwindow.h"
#include "editor/windows/meshimporter.h"
#include "editor/windows/projectexplorer.h"

uint32_t h_editor::Editor::init(const h_core::project::Project& project, const std::string& projectBasePath) {
    m_window = new h_core::Window();
    m_window->init("hydrogen editor - " + project.name, 1600, 900, true);

    h_core::theming::editor();

    h_editor::windows::ProjectExplorer* explorer = new h_editor::windows::ProjectExplorer(this, projectBasePath);
    m_windows.push_back(explorer);

//    explorer->registerNewAssetOpener(
//        "hymodel",
//        h_editor::windows::AssetOpener {
//            "Open Model...", [](h_editor::Editor* editor, const std::string& assetPath) -> AssetEditorWindow* {
//                HYLOG_INFO("EXPLORER: Opening %s", assetPath.c_str());
//                // TODO: model viewer
//                return nullptr;
//            } });

    auto makeMeshImporter = [](h_editor::Editor* editor, const std::string& assetPath) -> AssetEditorWindow* {
        return new h_editor::windows::MeshImporter(editor);
    };
    h_editor::windows::AssetOpener meshImporterOpener = h_editor::windows::AssetOpener { "Import Model...", makeMeshImporter };

    explorer->registerNewAssetOpener("gltf", meshImporterOpener);
    explorer->registerNewAssetOpener("glb", meshImporterOpener);
    explorer->registerNewAssetOpener("fbx", meshImporterOpener);

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

                default:
                    break;
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
        window->doGUI(); // Borked :(
    }

    for (h_editor::EditorWindow* window : m_windowsToClose) {
        m_windows.erase(std::find(m_windows.begin(), m_windows.end(), window));
    }
    m_windowsToClose.clear();

    for (h_editor::EditorWindow* window : m_windowsToAdd) {
        m_windows.push_back(window);
    }
    m_windowsToAdd.clear();

    if (m_modalOpen) {
        m_modalOpen = false;
        ImGui::OpenPopup("Hydrogen Editor Modal");
    }

    if (ImGui::BeginPopupModal("Hydrogen Editor Modal", nullptr, ImGuiWindowFlags_NoMove)) {
        ImGui::Text("%s", m_modalText.c_str());
        if (ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void h_editor::Editor::addNewWindow(h_editor::EditorWindow* window) {
    m_windowsToAdd.push_back(window);
}

void h_editor::Editor::closeWindow(h_editor::EditorWindow* window) {
    m_windowsToClose.push_back(window);
}

void h_editor::Editor::openModal(const std::string& text) {
    m_modalOpen = true;
    m_modalText = text;
}
