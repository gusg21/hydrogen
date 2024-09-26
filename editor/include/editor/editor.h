//
// Created by angus.goucher on 9/8/2024.
//

#pragma once

#include <stdint.h>

#include "core/project/project.h"
#include "core/window.h"
#include "editorwindow.h"

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

namespace h_editor {
class Editor {
  public:
    Editor() = default;

    uint32_t init(const h_core::project::Project& project, const std::string& projectBasePath);
    void run();
    void destroy();

    std::string getProjectBasePath();
    h_core::project::Project* getProject();
    [[nodiscard]] h_core::input::Input* getInput() const;
    [[nodiscard]] double getDeltaSecs() const;

    void addNewWindow(h_editor::EditorWindow* window);
    void closeWindow(h_editor::EditorWindow* window);
    void openModal(const std::string& text);
    void doGUI();

  private:
    std::string m_projectBasePath {};
    h_core::project::Project m_project {};

    h_core::Window* m_window = nullptr;
    h_core::EventQueue m_events {};
    h_core::input::Input* m_input = nullptr;

    uint32_t m_deltaNanosecs = 0;

    std::vector<h_editor::EditorWindow*> m_windows {};
    std::vector<h_editor::EditorWindow*> m_windowsToClose {};
    std::vector<h_editor::EditorWindow*> m_windowsToAdd {};
    bool m_modalOpen = false;
    std::string m_modalText {};
};
}  // namespace h_editor