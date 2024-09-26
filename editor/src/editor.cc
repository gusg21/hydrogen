//
// Created by angus.goucher on 9/8/2024.
//

#include "editor/editor.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>
#include <glad/glad.h>
#include <imgui.h>

#include "core/engineevents.h"
#include "core/input/input.h"
#include "core/log.h"
#include "core/theming/theming.h"
#include "editor/asseteditorwindow.h"
#include "editor/windows/meshimporter.h"
#include "editor/windows/projectexplorer.h"
#include "editor/windows/sceneeditor.h"

uint32_t h_editor::Editor::init(const h_core::project::Project& project, const std::string& projectBasePath) {
    // Store project
    m_project = project;
    m_projectBasePath = projectBasePath;

    // Window
    m_window = new h_core::Window();
    m_window->init("hydrogen editor - " + project.name, 1600, 900, true);

    // Input manager
    m_input = new h_core::input::Input();
    m_input->init(&m_project, m_window);

    // Imgui Style
    h_core::theming::editor();

    // Editor windows
    h_editor::windows::ProjectExplorer* explorer = new h_editor::windows::ProjectExplorer(this, projectBasePath);
    m_windows.push_back(explorer);

    auto makeMeshImporter = [](h_editor::Editor* editor) -> AssetEditorWindow* {
        return new h_editor::windows::MeshImporter(editor);
    };
    auto batchMakeMeshImporter = [](h_editor::Editor* editor, const std::string& assetPath) -> void {
        h_editor::windows::MeshImporter* imp = new h_editor::windows::MeshImporter(editor);
        HYLOG_INFO("BATCH MESH: Opening and importing %s", assetPath.c_str());
        imp->openAndImportDefault(assetPath);
        delete imp;
    };
    h_editor::windows::AssetOpener meshImporterOpener =
        h_editor::windows::AssetOpener { "Import Model...", makeMeshImporter, batchMakeMeshImporter };

    explorer->registerNewAssetOpener("gltf", meshImporterOpener);
    explorer->registerNewAssetOpener("glb", meshImporterOpener);
    explorer->registerNewAssetOpener("fbx", meshImporterOpener);

    auto openNewSceneEditor = [](h_editor::Editor* editor) -> AssetEditorWindow* {
        return new h_editor::windows::SceneEditor(editor);
    };
    h_editor::windows::AssetOpener sceneSpecOpener =
        h_editor::windows::AssetOpener { "Open Scene...", openNewSceneEditor, nullptr };
    explorer->registerNewAssetOpener("hyscene", sceneSpecOpener);

    return 0;
}

void h_editor::Editor::run() {
    std::chrono::time_point frameBeginTime = std::chrono::high_resolution_clock::now();

    bool running = true;
    while (running) {
        m_window->postEventsToQueue(&m_events);

        m_input->setMouseDelta(0, 0);

        for (uint32_t eventIndex = 0; eventIndex < m_events.getSize(); eventIndex++) {
            const h_core::Event* event = &m_events.getHeadPointer()[eventIndex];
            switch (event->type) {
                case ENGINE_EVENT_QUIT:
                    running = false;
                    break;
                case ENGINE_EVENT_MOUSE_MOTION:
                    m_input->setMousePos(event->mouseX, event->mouseY);
                    m_input->setMouseDelta(event->mouseDx, event->mouseDy);
                    break;

                default:
                    break;
            }
        }

        ::ImGui_ImplOpenGL3_NewFrame();
        ::ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        doGUI();

        ::glClearColor(0.f, 0.f, 0.f, 1.0f);
        ::glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ::ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update the window
        m_window->swap();

        // Clear the queue
        m_events.clear();

        // Update input
        m_input->updateInternals();

        auto frameEndTime = std::chrono::high_resolution_clock::now();
        m_deltaNanosecs = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEndTime - frameBeginTime).count();
        m_deltaNanosecs = MATH_MAX(m_deltaNanosecs, 1);
        frameBeginTime = frameEndTime;
    }
}

void h_editor::Editor::destroy() {}

std::string h_editor::Editor::getProjectBasePath() {
    return m_projectBasePath;
}

h_core::project::Project* h_editor::Editor::getProject() {
    return &m_project;
}

h_core::input::Input* h_editor::Editor::getInput() const {
    return m_input;
}

void h_editor::Editor::doGUI() {
    ImGui::ShowDemoWindow();


    for (h_editor::EditorWindow* window : m_windows) {
        window->doGUI();  // Borked :(
    }

    for (h_editor::EditorWindow* window : m_windowsToClose) {
        std::vector<h_editor::EditorWindow*>::iterator windowIter =
            std::find(m_windows.begin(), m_windows.end(), window);
        if (windowIter != m_windows.end()) { m_windows.erase(windowIter); }
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
        if (ImGui::Button("OK")) { ImGui::CloseCurrentPopup(); }

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

double h_editor::Editor::getDeltaSecs() const {
    return (double)m_deltaNanosecs / 1'000'000'000.0;
}