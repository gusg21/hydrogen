//
// Created by angus.goucher on 9/13/2024.
//

#include "editor/windows/projectexplorer.h"

#include <regex>

#include <SDL.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "core/log.h"
#include "core/theming/guicolors.h"
#include "editor/editor.h"
#include "editor/errors.h"
#include "editor/platform/path.h"

h_editor::windows::ProjectExplorer::ProjectExplorer(h_editor::Editor* editor, const std::string& projectBasePath)
    : h_editor::EditorWindow(editor, "Project Explorer") {
    m_projectPath = projectBasePath;
    m_browsingPath = "";
    HYLOG_INFO("PROJECT EXPLORER: Project base path is %s", m_projectPath.c_str());
}

void h_editor::windows::ProjectExplorer::paintContent() {
    // EditorWindow::paintContent();

    if (ImGui::BeginChild("Explorer", ImVec2 { -150, -0.1f })) {

        // Path Input
        ImGui::InputText("Path", &m_browsingPath);
        ImGui::SameLine();
        if (ImGui::Button("Home")) {
            m_browsingPath = "";
            m_currentSelection.clear();
        }
        std::vector<h_editor::platform::FileEntry> files {};
        uint32_t folderListingResult = h_editor::platform::getFolderListing(&files, m_projectPath + m_browsingPath);

        ImGui::Separator();

        // Tools
        if (ImGui::Button("Select By...")) {
            ImGui::OpenPopup("Select By");
            m_currentSelection.clear();
        }

        if (ImGui::BeginPopup("Select By")) {
            m_currentSelection.clear();

            ImGui::InputText("Regex", &m_selectByEntry);

            std::regex regex;
            bool regexOk = false;
            try {
                regex = std::regex { m_selectByEntry };
                regexOk = true;
            }
            catch (const std::regex_error& e) {
                regexOk = false;
            }

            if (regexOk) {
                for (h_editor::platform::FileEntry entry : files) {
                    std::smatch m;
                    if (entry.type == platform::FileType::FILE) {
                        if (std::regex_match(entry.name, m, regex)) {
                            m_currentSelection.push_back(entry.name);
                        }
                    }
                }
            } else {
                ImGui::SameLine();
                ImGui::TextColored(IMGUI_COLOR_WARN, "Bad Regex");
            }

            if (ImGui::Button("Select")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Close")) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Select All")) {
            for (h_editor::platform::FileEntry entry : files) {
                if (entry.type == platform::FileType::FILE) {
                    m_currentSelection.push_back(entry.name);
                }
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Select None")) {
            m_currentSelection.clear();
        }

        // File listing
        if (ImGui::BeginListBox("File Listing", ImVec2(-FLT_MIN, -FLT_MIN))) {
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

                        auto fileIter = std::find(m_currentSelection.begin(), m_currentSelection.end(), file.name);
                        bool fileSelected = fileIter != m_currentSelection.end();
                        if (ImGui::Selectable(entryText.c_str(), fileSelected)) {
                            if (fileSelected) { m_currentSelection.erase(fileIter); }
                            else { m_currentSelection.push_back(file.name); }
                        }
                        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) &&
                            file.type == h_editor::platform::FileType::DIRECTORY) {
                            // Navigate to directory
                            m_browsingPath += file.name + h_editor::platform::getPathSeparator();
                            m_browsingPath = h_editor::platform::canonicalizePath(m_browsingPath);
                            m_currentSelection.clear();
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
    }
    ImGui::EndChild();

    ImGui::SameLine();
    if (ImGui::BeginChild("File Controls", ImVec2 { -0.1f, -0.1f })) {
        if (m_currentSelection.empty()) {
            // Nothing to do anything with
            ImGui::TextWrapped("Nothing selected");
        }
        else {
            for (const std::string& selection : m_currentSelection) {
                // Individual file options
                std::string fullPath = m_projectPath + m_browsingPath + selection;

                ImGui::SeparatorText(selection.c_str());

                if (h_editor::platform::getFileType(fullPath) == platform::FileType::DIRECTORY) {
                    // Can't do anything with a directory
                    ImGui::TextWrapped("Directory selected");
                }
                else {
                    // Show file options based off extension
                    std::string extension = h_editor::platform::getFileExtension(selection);

                    if (m_assetOpenerLut.find(extension) == m_assetOpenerLut.end()) {
                        ImGui::TextWrapped("No tool associated with extension %s", extension.c_str());
                    }
                    else {
                        AssetOpener opener = m_assetOpenerLut[extension];
                        if (ImGui::Button(opener.openButtonText.c_str())) {
                            HYLOG_INFO("PROJECT EXPLORER: Calling opener...");

                            AssetEditorWindow* window = opener.openFunc(getEditor(), fullPath);
                            if (window != nullptr) {
                                getEditor()->addNewWindow(window);
                                window->open(m_browsingPath + selection);
                            }
                            else { getEditor()->openModal("Failed to create tool for specified asset."); }
                        }
                    }
                }
            }
        }
    }
    ImGui::EndChild();

    if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) { m_currentSelection.clear(); }
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
