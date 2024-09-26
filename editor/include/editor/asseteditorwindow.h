//
// Created by Angus Goucher on 9/15/2024.
//

#pragma once

#include <editor/editorwindow.h>
#include <string>

namespace h_editor {
class AssetEditorWindow : public h_editor::EditorWindow {
  public:
    AssetEditorWindow(h_editor::Editor* editor, const std::string& title) : h_editor::EditorWindow(editor, title) {}
    AssetEditorWindow(h_editor::Editor* editor, const std::string& title, h_core::math::Vector2 preferredSize)
        : h_editor::EditorWindow(editor, title, preferredSize) {}

    virtual void open(const std::string& assetPath) = 0;
    virtual void import() {};

    void doImportAndCloseButtons();
};
}  // namespace h_editor
