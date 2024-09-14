//
// Created by angus.goucher on 9/13/2024.
//

#pragma once

#include <string>

namespace h_editor {
class EditorWindow {
  public:
    EditorWindow() = delete;
    explicit EditorWindow(const std::string& title) : m_title(title) {}

    void doGUI();
    virtual void paintContent();

  protected:
    std::string m_title { "Unknown Window" };
};
}  // namespace h_editor