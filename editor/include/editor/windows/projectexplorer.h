//
// Created by angus.goucher on 9/13/2024.
//

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "editor/editorwindow.h"
#include "editor/asseteditorwindow.h"
#include "editor/asseteditorwindow.h"

#define BROWSING_PATH_MAX_LENGTH 1024

namespace h_editor {
class Editor;

namespace windows {

typedef h_editor::AssetEditorWindow* (*AssetOpenerFunc)(h_editor::Editor* editor, const std::string& assetPath);

struct AssetOpener {
  public:
    AssetOpener() = default;
    AssetOpener(const std::string& openButtonText, AssetOpenerFunc openFunc)
        : openButtonText(openButtonText), openFunc(openFunc) {}

    AssetOpenerFunc openFunc = nullptr;
    std::string openButtonText { "Open Asset..." };
};

class ProjectExplorer : public h_editor::EditorWindow {
  public:
    explicit ProjectExplorer(h_editor::Editor* editor, const std::string& projectBasePath);

    void paintContent() override;
    void paintPopupsAndModals() override;
    void registerNewAssetOpener(const std::string& extension, AssetOpener opener);

  private:
    std::string m_browsingPath {};
    std::string m_projectPath {};
    std::string m_currentSelection {};

    std::unordered_map<std::string, AssetOpener> m_assetOpenerLut {};
};
}  // namespace windows
}  // namespace h_editor