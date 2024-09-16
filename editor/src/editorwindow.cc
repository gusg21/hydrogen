//
// Created by angus.goucher on 9/13/2024.
//

#include "editor/editorwindow.h"

#include <imgui.h>

#include "core/theming/guicolors.h"

void h_editor::EditorWindow::doGUI() {
    if (ImGui::Begin(m_title.c_str())) {
        paintContent();
    }
    ImGui::End();

    paintPopupsAndModals();
}

void h_editor::EditorWindow::paintContent() {
    ImGui::TextColored(IMGUI_COLOR_WARN, "paintContent() not overridden!");
}

h_editor::Editor* h_editor::EditorWindow::getEditor() {
    return m_editor;
}

void h_editor::EditorWindow::paintPopupsAndModals() {

}
