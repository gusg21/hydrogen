//
// Created by angus.goucher on 9/13/2024.
//

#include "editor/windows/meshimporter.h"

#include <imgui.h>

void h_editor::windows::MeshImporter::open(const std::string& meshFile) {
    this->meshFile = meshFile;
}
void h_editor::windows::MeshImporter::paintContent() {
    // EditorWindow::paintContent();

    ImGui::Text("Editing %s", meshFile.c_str());
}