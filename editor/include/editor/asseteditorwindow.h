//
// Created by Angus Goucher on 9/15/2024.
//

#pragma once

#include <string>
#include <editor/editorwindow.h>

namespace h_editor {
class AssetEditorWindow : public h_editor::EditorWindow {
  public:
    explicit AssetEditorWindow(h_editor::Editor* editor, const std::string& title) : h_editor::EditorWindow(editor, title) {}

    virtual void open(const std::string& assetPath) = 0;
    virtual void import() {};

    void doImportAndCloseButtons();
};
}
