//
// Created by angus.goucher on 9/13/2024.
//

#pragma once

#include <string>

#include "core/math/math.h"

namespace h_editor {
class Editor;

class EditorWindow {
  public:
    EditorWindow() = delete;
    EditorWindow(h_editor::Editor* editor, const std::string& title) : m_title(title), m_editor(editor) {}
    EditorWindow(h_editor::Editor* editor, const std::string& title, h_core::math::Vector2 preferredSize)
        : m_title(title), m_preferredSize(preferredSize), m_editor(editor) {}

    void doGUI();
    virtual void close();
    virtual void paintContent();
    virtual void paintPopupsAndModals();

  protected:
    h_editor::Editor* getEditor() const;

    std::string m_title { "Unknown Window" };
    h_core::math::Vector2 m_preferredSize { 900, 700 };

  private:
    h_editor::Editor* m_editor = nullptr;
};
}  // namespace h_editor