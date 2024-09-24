//
// Created by angus.goucher on 9/13/2024.
//

#include "editor/windows/projectexplorer.h"

#include <regex>

#include <SDL.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "core/assets.h"
#include "core/log.h"
#include "core/theming/guicolors.h"
#include "editor/editor.h"
#include "editor/errors.h"
#include "editor/platform/path.h"

h_editor::windows::ProjectExplorer::ProjectExplorer(h_editor::Editor* editor, const std::string& projectBasePath)
    : h_editor::EditorWindow(editor, "Project Explorer") {
    m_projectPath = projectBasePath;
    m_browsingPath = h_editor::platform::getPathSeparator();
    HYLOG_INFO("PROJECT EXPLORER: Project base path is %s", m_projectPath.c_str());
}

void h_editor::windows::ProjectExplorer::paintContent() {
    // EditorWindow::paintContent();

    h_core::project::Project* project = getEditor()->getProject();

    ImGui::SeparatorText("File View");
    if (ImGui::BeginChild("File Explorer", ImVec2 { -0.1, 400.f }, ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY)) {
        if (ImGui::BeginChild("Explorer", ImVec2 { -250, -0.1f }, ImGuiChildFlags_ResizeX)) {
            // Path Input
            ImGui::InputText("Path", &m_browsingPath);
            ImGui::SameLine();
            if (ImGui::Button("Home")) {
                m_browsingPath = "";
                m_currentSelection.clear();
            }
            ImGui::SameLine();
            ImGui::BeginDisabled(m_browsingPath.empty());
            if (ImGui::Button("Up")) {
                HYLOG_DEBUG("PROJECT EXPLORER: up path PRE %s", m_browsingPath.c_str());
                m_browsingPath = m_browsingPath + ".." + h_editor::platform::getPathSeparator();
                HYLOG_DEBUG("PROJECT EXPLORER: up path APP %s", m_browsingPath.c_str());
                m_browsingPath = h_editor::platform::canonicalizePath(m_browsingPath);
                HYLOG_DEBUG("PROJECT EXPLORER: up path CANON %s", m_browsingPath.c_str());
            }
            ImGui::EndDisabled();

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

                            if (entry.type == h_editor::platform::FileType::FILE) {
                                if (project->hasAssetPath(m_browsingPath + entry.name)) {
                                    if (ImGui::SmallButton(("-##" + entry.name).c_str())) {
                                        project->removeByPath(m_browsingPath + entry.name);
                                    }
                                }
                                else {
                                    if (ImGui::SmallButton(("+##" + entry.name).c_str())) {
                                        setAddAssetDefaults(m_browsingPath + entry.name);

                                        if (ImGui::IsKeyDown(ImGuiKey_ModShift)) {
                                            ImGui::OpenPopup("Add Asset Details");
                                        } else {
                                            doAddAsset();
                                        }
                                    }
                                }
                            }
                            else {
                                ImGui::BeginDisabled();
                                ImGui::SmallButton(" ");
                                ImGui::EndDisabled();
                            }
                            ImGui::SameLine();


                            uint32_t selectedFileIndex = -1;
                            for (uint32_t fileIndex = 0; fileIndex < m_currentSelection.size(); fileIndex++) {
                                if (m_currentSelection[fileIndex].name == entry.name) { selectedFileIndex = fileIndex; }
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

                        // Add Asset Popup
                        if (ImGui::BeginPopup("Add Asset Details")) {
                            if (!m_addRemoteEntry) { ImGui::InputText("Path", &m_addAssetPathEntry); }
                            ImGui::InputInt("Type", (int*)&m_addTypeEntry);
                            ImGui::InputInt("Index", (int*)&m_addIndexEntry);
                            ImGui::Checkbox("Remote", &m_addRemoteEntry);

                            if (ImGui::Button("Add")) { doAddAsset(); ImGui::CloseCurrentPopup(); }
                            ImGui::SameLine();
                            if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }

                            ImGui::EndPopup();
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

                                    AssetEditorWindow* window = opener.openFunc(getEditor());
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
                    if (ImGui::BeginChild("Batch File Controls", ImVec2 { -0.1f, 100.f }, ImGuiChildFlags_Border)) {
                        ImGui::SeparatorText("Batch Opener");

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
                }

                if (m_currentSelection.size() > 1) {
                    if (ImGui::BeginChild("Batch Project Adder", ImVec2 { -0.1f, 100.f }, ImGuiChildFlags_Border)) {
                        ImGui::SeparatorText("Batch Project Manip");

                        if (ImGui::Button("Add To Project")) {
                            for (const platform::FileEntry& entry : m_currentSelection) {
                                if (entry.type == platform::FileType::FILE) {
                                    if (!project->hasAssetPath(m_browsingPath + entry.name)) {
                                        setAddAssetDefaults(m_browsingPath + entry.name);
                                        doAddAsset();
                                    }
                                }
                            }
                        }
                        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                            ImGui::SetTooltip("Uses inferred types and next available index");
                        }
                    }
                    ImGui::EndChild();
                }
            }
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();

    ImGui::SeparatorText("Project View");
    if (ImGui::BeginChild("Project Explorer", ImVec2 {}, ImGuiChildFlags_Border)) {
        ImGui::Text("Asset Count: %d", project->requiredAssets.size());

        ImGui::Separator();

        if (ImGui::BeginTable(
                "Projct Asset List", 4,
                ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY)) {
            ImGui::TableSetupColumn("Path", 0, 5);
            ImGui::TableSetupColumn("Type", 0, 2);
            ImGui::TableSetupColumn("Index", 0, 2);
            ImGui::TableSetupColumn("Remote", 0, 1);
            ImGui::TableSetupScrollFreeze(4, 1);

            ImGui::TableHeadersRow();

            for (h_core::project::ProjectAssetEntry& projectAsset : project->requiredAssets) {
                ImGui::TableNextRow();

                ImGui::TableNextColumn();
                {
                    ImGui::PushItemWidth(-1);
                    if (projectAsset.isRemote) { ImGui::TextDisabled("Remote Asset"); }
                    else {
                        ImGui::InputText(
                            ("##path" + std::to_string(projectAsset.index)).c_str(), &projectAsset.assetPath);
                    }
                    ImGui::PopItemWidth();
                }
                ImGui::TableNextColumn();
                {
                    ImGui::PushItemWidth(-1);
                    ImGui::InputInt(
                        ("##type" + std::to_string(projectAsset.index)).c_str(),
                        reinterpret_cast<int*>(&projectAsset.typeId));
                    ImGui::TextDisabled("%s", h_core::Assets::getAssetTypeName(projectAsset.typeId));

                    ImGui::PopItemWidth();
                }
                ImGui::TableNextColumn();
                {
                    ImGui::PushItemWidth(-1);
                    ImGui::InputInt(
                        ("##index" + std::to_string(projectAsset.index)).c_str(),
                        reinterpret_cast<int*>(&projectAsset.index));
                    ImGui::PopItemWidth();
                }
                ImGui::TableNextColumn();
                { ImGui::Checkbox(("##remote" + std::to_string(projectAsset.index)).c_str(), &projectAsset.isRemote); }
            }

            ImGui::EndTable();
        }

        // uint32_t itemWidth = 70.f;
        // uint32_t width = ImGui::GetContentRegionAvail().x;
        // if (ImGui::BeginTable(
        //         "Project Explorer Files", width / itemWidth, 0,
        //         ImVec2 { static_cast<float>((width / itemWidth) * itemWidth), -0.1f })) {  //
        //         NOLINT(*-integer-division)
        //     for (const h_core::project::ProjectAssetEntry& projectAsset : project->requiredAssets) {
        //         ImGui::TableNextColumn();
        //
        //         // File
        //         if (ImGui::BeginChild(projectAsset.index, ImVec2 { static_cast<float>(itemWidth), 70.f })) {
        //             ImGui::TextWrapped("%s",
        //             projectAsset.assetPath.substr(projectAsset.assetPath.find_last_of('/')).c_str());
        //         }
        //         ImGui::EndChild();
        //     }
        //
        //     ImGui::EndTable();
        // }
    }
    ImGui::EndChild();

    if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) { m_currentSelection.clear(); }
}

void h_editor::windows::ProjectExplorer::registerNewAssetOpener(
    const std::string& extension, const h_editor::windows::AssetOpener& opener) {
    m_assetOpenerLut[extension] = opener;
}
void h_editor::windows::ProjectExplorer::setAddAssetDefaults(const std::string& path) {
    m_addAssetPathEntry = path;
    m_addIndexEntry = getEditor()->getProject()->getOpenIndex();
    m_addTypeEntry = h_core::Assets::determineAssetTypeFromExtension(h_editor::platform::getFileExtension(path));
    m_addRemoteEntry = false;
}

void h_editor::windows::ProjectExplorer::doAddAsset() {
    if (m_addTypeEntry != UINT32_MAX) {
        getEditor()->getProject()->requiredAssets.emplace_back(
        m_addIndexEntry, m_addTypeEntry, m_addAssetPathEntry, m_addRemoteEntry);
    }
}


void h_editor::windows::ProjectExplorer::paintPopupsAndModals() {}
