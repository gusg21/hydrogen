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

    void addNewWindow(h_editor::EditorWindow* window);
    void doGUI();

  private:
    h_core::Window* m_window = nullptr;
    h_core::EventQueue m_events {};

    std::vector<h_editor::EditorWindow*> m_windows {};
};
}  // namespace h_editor