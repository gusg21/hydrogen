//
// Created by angus.goucher on 9/8/2024.
//

#pragma once

#include <stdint.h>

#include "core/project/project.h"
#include "core/window.h"
#include "editorwindow.h"

namespace h_editor {
class Editor {
  public:
    Editor() = default;

    uint32_t init(const h_core::project::Project& project, const std::string& projectBasePath);
    void run();
    void destroy();

    std::string getProjectBasePath();

    void addNewWindow(h_editor::EditorWindow* window);
    void closeWindow(h_editor::EditorWindow* window);
    void openModal(const std::string& text);
    void doGUI();

  private:
    h_core::Window* m_window = nullptr;
    h_core::EventQueue m_events {};

    std::vector<h_editor::EditorWindow*> m_windows {};
    std::vector<h_editor::EditorWindow*> m_windowsToClose {};
    std::vector<h_editor::EditorWindow*> m_windowsToAdd {};
    bool m_modalOpen = false;
    std::string m_modalText {};
    std::string m_projectBasePath {};
};
}  // namespace h_editor