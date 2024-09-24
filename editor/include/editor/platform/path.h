//
// Created by Angus Goucher on 9/15/2024.
//

#pragma once

#include <string>
#include <vector>

namespace h_editor {
namespace platform {
enum class FileType { FILE, DIRECTORY };

struct FileEntry {
    FileEntry(const std::string& name, FileType type) : name(name), type(type) {}

    std::string name {};
    FileType type = FileType::FILE;
};

uint32_t getFolderListing(std::vector<h_editor::platform::FileEntry>* out_entries, const std::string& path);
std::string canonicalizePath(const std::string& path);
constexpr char getPathSeparator();
FileType getFileType(const std::string& path);
std::string getFileExtension(const std::string& path);
std::string getDirectoryFromPath(const std::string& path);
std::string getBaseFromPath(const std::string& path);

}  // namespace platform
}  // namespace h_editor