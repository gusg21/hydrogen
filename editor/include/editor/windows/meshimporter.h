//
// Created by angus.goucher on 9/13/2024.
//

#pragma once

#include "editor/asseteditorwindow.h"

#define MESHIMPORTER_ENTRY_LENGTH 1024

namespace h_editor {
namespace windows {
class MeshImporter : public h_editor::AssetEditorWindow {
  public:
    explicit MeshImporter(h_editor::Editor* editor) : AssetEditorWindow(editor, "Mesh Importer") {}

    void open(const std::string& meshFile) override;
    void import() override;
    void paintContent() override;

  private:
    void openFbx();
    void openGltf(bool binary);

    std::string getGlbPath();

    std::string m_meshFile {};
    char m_fbx2gltfEntry[MESHIMPORTER_ENTRY_LENGTH] = {};
    bool m_needsFbx2Gltf = false;
    char m_basePathEntry[MESHIMPORTER_ENTRY_LENGTH] = {};
    bool m_needsBasePath = false;
    char m_outputPathEntry[MESHIMPORTER_ENTRY_LENGTH] = {};
};
}  // namespace windows
}  // namespace h_editor
