//
// Created by Angus Goucher on 9/17/2024.
//
#include "editor/asseteditorwindow.h"

#include <imgui.h>

void h_editor::AssetEditorWindow::doImportAndCloseButtons() {
    if (ImGui::Button("Import")) { import(); }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) { close(); }
}
