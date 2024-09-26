//
// Created by angus.goucher on 9/13/2024.
//

#include "editor/editorwindow.h"

#include <imgui.h>

#include "core/theming/guicolors.h"
#include "editor/editor.h"

void h_editor::EditorWindow::doGUI() {
    ImGui::SetNextWindowSize(ImVec2 { m_preferredSize.x, m_preferredSize.y }, ImGuiCond_Appearing);
    if (ImGui::Begin(
            (m_title + "##" + std::to_string(reinterpret_cast<uintptr_t>(this))).c_str(), nullptr,
            ImGuiWindowFlags_MenuBar)) {
        paintContent();
    }
    ImGui::End();

    paintPopupsAndModals();
}

void h_editor::EditorWindow::paintContent() {
    ImGui::TextColored(IMGUI_COLOR_WARN, "paintContent() not overridden!");
}

h_editor::Editor* h_editor::EditorWindow::getEditor() const {
    return m_editor;
}

void h_editor::EditorWindow::paintPopupsAndModals() {}

void h_editor::EditorWindow::close() {
    m_editor->closeWindow(this);
}
