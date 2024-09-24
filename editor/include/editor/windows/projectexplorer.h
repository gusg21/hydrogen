//
// Created by angus.goucher on 9/13/2024.
//

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "editor/asseteditorwindow.h"
#include "editor/editorwindow.h"
#include "editor/platform/path.h"

#define BROWSING_PATH_MAX_LENGTH 1024

namespace h_editor {
class Editor;

namespace windows {

typedef h_editor::AssetEditorWindow* (*AssetOpenerFunc)(h_editor::Editor* editor, const std::string& assetPath);
typedef void (*AssetBatchOpenerFunc)(h_editor::Editor* editor, const std::string& assetPath);

struct AssetOpener {
    AssetOpener() = default;
    AssetOpener(const std::string& openButtonText, AssetOpenerFunc openFunc, AssetBatchOpenerFunc batchOpenFunc)
        : openButtonText(openButtonText), openFunc(openFunc), batchOpenFunc(batchOpenFunc) {}

    AssetOpenerFunc openFunc = nullptr;
    AssetBatchOpenerFunc batchOpenFunc = nullptr;
    std::string openButtonText { "Open Asset..." };
};

class ProjectExplorer : public h_editor::EditorWindow {
  public:
    explicit ProjectExplorer(h_editor::Editor* editor, const std::string& projectBasePath);

    void paintContent() override;
    void paintPopupsAndModals() override;
    void registerNewAssetOpener(const std::string& extension, const h_editor::windows::AssetOpener& opener);

  private:
    std::string m_browsingPath {};
    std::string m_projectPath {};
    std::vector<h_editor::platform::FileEntry> m_currentSelection {};
    std::string m_selectByEntry {};

    std::unordered_map<std::string, AssetOpener> m_assetOpenerLut {};
    bool m_openPopup = false;
};
}  // namespace windows
}  // namespace h_editor