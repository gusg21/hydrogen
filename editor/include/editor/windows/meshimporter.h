//
// Created by angus.goucher on 9/13/2024.
//

#pragma once

#include "editor/editorwindow.h"

namespace h_editor {
namespace windows {
class MeshImporter : public EditorWindow {
  public:
    MeshImporter() : EditorWindow("Mesh Importer") {}

    void open(const std::string& meshFile);
    void paintContent() override;

  private:
    std::string meshFile {};
};
}  // namespace windows
}  // namespace h_editor
