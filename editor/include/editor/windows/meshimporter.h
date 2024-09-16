//
// Created by angus.goucher on 9/13/2024.
//

#pragma once

#include "editor/asseteditorwindow.h"

namespace h_editor {
namespace windows {
class MeshImporter : public h_editor::AssetEditorWindow {
  public:
    explicit MeshImporter(h_editor::Editor* editor) : AssetEditorWindow(editor, "Mesh Importer") {}

    void open(const std::string& meshFile) override;
    void paintContent() override;

  private:
    std::string meshFile {};
};
}  // namespace windows
}  // namespace h_editor
