//
// Created by angus.goucher on 9/13/2024.
//

#include "editor/windows/projectexplorer.h"

#include <SDL.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "core/assets.h"
#include "core/log.h"
#include "core/theming/guicolors.h"
#include "editor/errors.h"
#include "editor/platform/path.h"
#include "editor/editor.h"

h_editor::windows::ProjectExplorer::ProjectExplorer(h_editor::Editor* editor, const std::string& projectBasePath)
    : h_editor::EditorWindow(editor, "Project Explorer") {
    m_projectPath = projectBasePath;
    m_browsingPath = "";
    HYLOG_INFO("PROJECT EXPLORER: Project base path is %s", m_projectPath.c_str());
}

void h_editor::windows::ProjectExplorer::paintContent() {
    // EditorWindow::paintContent();

    if (ImGui::BeginChild("Explorer", ImVec2 { -150, -0.1f })) {
        ImGui::InputText("Path", &m_browsingPath);

        ImGui::SameLine();
        if (ImGui::Button("Home")) { m_browsingPath = ""; }

        std::vector<h_editor::platform::FileEntry> files {};
        uint32_t folderListingResult = h_editor::platform::getFolderListing(&files, m_projectPath + m_browsingPath);
        ImGui::PushID(6);
        if (ImGui::BeginListBox("", ImVec2(-FLT_MIN, -FLT_MIN))) {
            if (folderListingResult == 0) {
                if (files.empty()) { ImGui::TextDisabled("Empty Directory"); }
                else {
                    for (const h_editor::platform::FileEntry& file : files) {
                        std::string entryText = file.name;
                        if (file.type == h_editor::platform::FileType::DIRECTORY) {
                            entryText += h_editor::platform::getPathSeparator();
                        }
                        ImGui::PushStyleColor(
                            ImGuiCol_Text, file.type == h_editor::platform::FileType::DIRECTORY
                                               ? ImVec4 { 1.0f, 1.0f, 1.0f, 0.5f }
                                               : ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f });
                        if (ImGui::Selectable(entryText.c_str(), m_currentSelection == file.name)) {
                            m_currentSelection = file.name;
                        }
                        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) &&
                            file.type == h_editor::platform::FileType::DIRECTORY) {
                            m_browsingPath += file.name + h_editor::platform::getPathSeparator();
                            m_browsingPath = h_editor::platform::canonicalizePath(m_browsingPath);
                        }
                        ImGui::PopStyleColor();
                    }
                }
            }
            else if (folderListingResult == ERROR_PATH_INVALID) {
                ImGui::TextColored(IMGUI_COLOR_WARN, "Invalid path");
            }
            else { ImGui::TextColored(IMGUI_COLOR_ERROR, "Error listing files"); }

            ImGui::EndListBox();
        }
        ImGui::PopID();
    }
    ImGui::EndChild();

    ImGui::SameLine();
    if (ImGui::BeginChild("File Controls", ImVec2 { -0.1f, -0.1f })) {
        std::string fullPath = m_projectPath + m_browsingPath + m_currentSelection;
        if (m_currentSelection.empty()) {
            // Nothing to do anything with
            ImGui::TextDisabled("Nothing selected");
        }
        else {
            if (h_editor::platform::getFileType(fullPath) == platform::FileType::DIRECTORY) {
                // Can't do anything with a directory
                ImGui::TextDisabled("Directory selected");
            }
            else {
                std::string extension = h_editor::platform::getFileExtension(m_currentSelection);

                if (m_assetOpenerLut.find(extension) == m_assetOpenerLut.end()) {
                    ImGui::TextDisabled("No tool associated with extension %s", extension.c_str());
                }
                else {
                    AssetOpener opener = m_assetOpenerLut[extension];
                    if (ImGui::Button(opener.openButtonText.c_str())) {
                        HYLOG_INFO("PROJECT EXPLORER: Calling opener...");

                        AssetEditorWindow* window = opener.openFunc(getEditor(), fullPath);
                        if (window != nullptr) {
                            getEditor()->addNewWindow(window);
                            window->open(m_browsingPath + m_currentSelection);
                        }
                        else {
                            getEditor()->openModal("Failed to create tool for specified asset.");
                        }

                        m_currentSelection.clear();
                    }
                }
            }
        }
    }
    ImGui::EndChild();

    if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        m_currentSelection.clear();
    }
}

void h_editor::windows::ProjectExplorer::registerNewAssetOpener(
    const std::string& extension, const h_editor::windows::AssetOpener& opener) {
    m_assetOpenerLut[extension] = opener;
}

void h_editor::windows::ProjectExplorer::paintPopupsAndModals() {
//    if (m_openPopup) {
//        ImGui::OpenPopup("Tool Creation Error");
//        m_openPopup = false;
//    }
//
//    // Popups and Modals
//    if (ImGui::BeginPopupModal("Tool Creation Error")) {
//        HYLOG_DEBUG("Tool Creation");
//        ImGui::Text("Failed to create tool for specified asset.");
//        if (ImGui::Button("OK")) {
//            ImGui::CloseCurrentPopup();
//        }
//
//        ImGui::EndPopup();
//    }

}
