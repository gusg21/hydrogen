//
// Created by angus.goucher on 9/13/2024.
//

#pragma once

#include <string>

namespace h_editor {
class Editor;

class EditorWindow {
  public:
    EditorWindow() = delete;
    explicit EditorWindow(h_editor::Editor* editor, const std::string& title) : m_editor(editor), m_title(title) {}

    void doGUI();
    void close();
    virtual void paintContent();
    virtual void paintPopupsAndModals();

  protected:
    h_editor::Editor* getEditor();

    std::string m_title { "Unknown Window" };

  private:
    h_editor::Editor* m_editor = nullptr;
};
}  // namespace h_editor