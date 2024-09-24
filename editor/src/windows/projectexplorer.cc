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

    if (ImGui::BeginChild("Explorer", ImVec2 { -150, -0.1f }, ImGuiChildFlags_ResizeX)) {
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
            } catch (const std::regex_error& e) { regexOk = false; }

            if (regexOk) {
                for (const h_editor::platform::FileEntry& entry : files) {
                    std::smatch m;
                    if (entry.type == platform::FileType::FILE) {
                        if (std::regex_match(entry.name, m, regex)) { m_currentSelection.push_back(entry); }
                    }
                }
            }
            else {
                // ReSharper disable once CppDFAUnreachableCode
                // Code's reachable, not sure why CLion thinks it isn't.
                ImGui::SameLine();
                ImGui::TextColored(IMGUI_COLOR_WARN, "Bad Regex");
            }

            if (ImGui::Button("Select")) { ImGui::CloseCurrentPopup(); }
            ImGui::SameLine();
            if (ImGui::Button("Close")) { ImGui::CloseCurrentPopup(); }

            ImGui::EndPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Select All")) {
            for (const h_editor::platform::FileEntry& entry : files) {
                if (entry.type == platform::FileType::FILE) { m_currentSelection.push_back(entry); }
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Select None")) { m_currentSelection.clear(); }

        // File listing
        if (ImGui::BeginListBox("File Listing", ImVec2(-FLT_MIN, -FLT_MIN))) {
            if (folderListingResult == 0) {
                if (files.empty()) { ImGui::TextDisabled("Empty Directory"); }
                else {
                    for (const h_editor::platform::FileEntry& entry : files) {
                        std::string entryText = entry.name;
                        if (entry.type == h_editor::platform::FileType::DIRECTORY) {
                            entryText += h_editor::platform::getPathSeparator();
                        }
                        ImGui::PushStyleColor(
                            ImGuiCol_Text, entry.type == h_editor::platform::FileType::DIRECTORY
                                               ? ImVec4 { 1.0f, 1.0f, 1.0f, 0.5f }
                                               : ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f });

                        uint32_t selectedFileIndex = -1;
                        for (uint32_t fileIndex = 0; fileIndex < m_currentSelection.size(); fileIndex++) {
                            if (m_currentSelection[fileIndex].name == entry.name) {
                                selectedFileIndex = fileIndex;
                            }
                        }
                        if (ImGui::Selectable(entryText.c_str(), selectedFileIndex != -1)) {
                            if (ImGui::IsKeyDown(ImGuiKey_ModShift)) {
                                if (selectedFileIndex != -1) {
                                    m_currentSelection.erase(m_currentSelection.begin() + selectedFileIndex);
                                }
                                else { m_currentSelection.push_back(entry); }
                            }
                            else {
                                m_currentSelection.clear();
                                m_currentSelection.push_back(entry);
                            }
                        }
                        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) &&
                            entry.type == h_editor::platform::FileType::DIRECTORY) {
                            // Navigate to directory
                            m_browsingPath += entry.name + h_editor::platform::getPathSeparator();
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
            std::string selectionCommonExtension {};
            bool hasCommonExtension = false;
            for (const platform::FileEntry& selection : m_currentSelection) {
                // Determine if all selected items have the same extension
                std::string extension = h_editor::platform::getFileExtension(selection.name);
                if (selection.type == platform::FileType::FILE) {
                    if (extension != selectionCommonExtension && !selectionCommonExtension.empty()) {
                        hasCommonExtension = false;
                    }
                    else if (selectionCommonExtension.empty()) {
                        selectionCommonExtension = extension;
                        hasCommonExtension = true;
                    }
                    else if (extension == selectionCommonExtension) {  // not needed, for clarity
                        hasCommonExtension = true;
                    }
                }

                // Individual file options
                std::string fullPath = m_projectPath + m_browsingPath + selection.name;

                ImGui::SeparatorText(selection.name.c_str());

                if (h_editor::platform::getFileType(fullPath) == platform::FileType::DIRECTORY) {
                    // Can't do anything with a directory
                    ImGui::TextWrapped("Directory selected");
                }
                else {
                    if (m_assetOpenerLut.find(extension) == m_assetOpenerLut.end()) {
                        ImGui::TextWrapped("No tool associated with extension %s", extension.c_str());
                    }
                    else {
                        // File open button
                        AssetOpener opener = m_assetOpenerLut[extension];
                        if (opener.openFunc != nullptr) {
                            if (ImGui::Button(opener.openButtonText.c_str())) {
                                HYLOG_INFO("PROJECT EXPLORER: Calling opener...");

                                AssetEditorWindow* window = opener.openFunc(getEditor(), fullPath);
                                if (window != nullptr) {
                                    getEditor()->addNewWindow(window);
                                    window->open(m_browsingPath + selection.name);
                                }
                                else { getEditor()->openModal("Failed to create tool for specified asset."); }
                            }
                        }
                    }
                }
            }

            if (hasCommonExtension) {
                ImGui::Indent();
                if (ImGui::BeginChild("Batch File Controls", ImVec2 {-0.1f, 100.f}, ImGuiChildFlags_Border)) {
                    ImGui::SeparatorText("Batch");

                    for (const std::pair<std::string, h_editor::windows::AssetOpener> opener : m_assetOpenerLut) {
                        if (opener.first == selectionCommonExtension) {
                            if (opener.second.batchOpenFunc != nullptr) {
                                // Batch open button
                                if (ImGui::Button(opener.second.openButtonText.c_str())) {
                                    for (const platform::FileEntry& path : m_currentSelection) {
                                        if (path.type == platform::FileType::FILE) {
                                            opener.second.batchOpenFunc(getEditor(), m_browsingPath + path.name);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                ImGui::EndChild();
                ImGui::Unindent();
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
