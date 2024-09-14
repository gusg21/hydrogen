//
// Created by angus.goucher on 9/13/2024.
//

#pragma once

#include <vector>


#include "editor/editorwindow.h"

namespace h_editor {
namespace windows {
enum class ProjectExplorerFileType {
    FILE, DIRECTORY
};

struct ProjectExplorerFileEntry {
    ProjectExplorerFileEntry(const std::string& name, ProjectExplorerFileType type) : name(name), type(type) {}

    std::string name {};
    ProjectExplorerFileType type = ProjectExplorerFileType::FILE;
};

class ProjectExplorer : public h_editor::EditorWindow {
  public:
    explicit ProjectExplorer(const std::string& projectBasePath);

    void paintContent() override;
    static std::vector<h_editor::windows::ProjectExplorerFileEntry> getFolderListing(const std::string& path);
    static std::string canonicalizePath(const std::string& path);

  private:
    std::string m_projectPath {};
    std::string m_browsingPath {};
    std::string m_currentSelection {};
};
}  // namespace windows
}  // namespace h_editor