//
// Created by angus.goucher on 9/13/2024.
//

#include "editor/windows/projectexplorer.h"

#include <SDL.h>
#include <Shlwapi.h>
#include <Windows.h>
#include <imgui.h>

#include "core/log.h"

h_editor::windows::ProjectExplorer::ProjectExplorer(const std::string& projectBasePath)
    : h_editor::EditorWindow("Project Explorer") {
    m_projectPath = projectBasePath;
    m_browsingPath = m_projectPath;
    HYLOG_INFO("PROJECT EXPLORER: Project base path is %s", m_projectPath.c_str());
}

void h_editor::windows::ProjectExplorer::paintContent() {
    // EditorWindow::paintContent();

    ImGui::Text("Base Path: %s", m_projectPath.c_str());
    const std::vector<ProjectExplorerFileEntry> files = getFolderListing(m_browsingPath);
    ImGui::PushID(6);
    if (ImGui::BeginListBox("", ImVec2(-FLT_MIN, -FLT_MIN))) {
        for (const ProjectExplorerFileEntry& file : files) {
            std::string entryText = file.name + (file.type == ProjectExplorerFileType::DIRECTORY ? "/" : "");
            ImGui::PushStyleColor(
                ImGuiCol_Text, file.type == ProjectExplorerFileType::DIRECTORY ? ImVec4 { 1.0f, 1.0f, 1.0f, 0.5f }
                                                                               : ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f });
            if (ImGui::Selectable(entryText.c_str(), m_currentSelection == file.name)) {
                m_currentSelection = file.name;
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) &&
                file.type == ProjectExplorerFileType::DIRECTORY) {
                m_browsingPath += file.name + "/";
                m_browsingPath = canonicalizePath(m_browsingPath);
            }
            ImGui::PopStyleColor();
        }
        ImGui::EndListBox();
    }
    ImGui::PopID();
}

#define PROJECTEXPLORER_MAX_PATH_LENGTH 1024

std::vector<h_editor::windows::ProjectExplorerFileEntry> h_editor::windows::ProjectExplorer::getFolderListing(
    const std::string& path) {
    std::string fixedPath = path;
    if (fixedPath.back() != '/') { fixedPath.push_back('/'); }
    std::vector<ProjectExplorerFileEntry> names {};
    char search_path[PROJECTEXPLORER_MAX_PATH_LENGTH] = {};
    sprintf(search_path, "%s*.*", fixedPath.c_str());
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path, &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            names.emplace_back(
                fd.cFileName, fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? ProjectExplorerFileType::DIRECTORY
                                                                             : ProjectExplorerFileType::FILE);
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return names;
}

std::string h_editor::windows::ProjectExplorer::canonicalizePath(const std::string& path) {
    char outPath[PROJECTEXPLORER_MAX_PATH_LENGTH] = {};
    PathCanonicalizeA(outPath, path.c_str());
    return std::string { outPath };
}