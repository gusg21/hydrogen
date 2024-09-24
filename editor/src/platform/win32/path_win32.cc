//
// Created by Angus Goucher on 9/15/2024.
//

#include "editor/platform/path.h"

#include "Shlwapi.h"
#include "editor/errors.h"

#define PLATFORM_WIN32_MAX_PATH_LENGTH 1024

uint32_t h_editor::platform::getFolderListing(std::vector<h_editor::platform::FileEntry>* out_entries,
    const std::string& path) {
    if (path.empty()) return ERROR_PATH_INVALID;

    std::string fixedPath = path;
    if (fixedPath.back() != getPathSeparator()) { fixedPath.push_back(getPathSeparator()); }
    char search_path[PLATFORM_WIN32_MAX_PATH_LENGTH] = {};
    sprintf(search_path, "%s*.*", fixedPath.c_str());
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path, &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            out_entries->emplace_back(
                fd.cFileName, fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? h_editor::platform::FileType::DIRECTORY
                                                                             : h_editor::platform::FileType::FILE);
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return 0;
}

std::string h_editor::platform::canonicalizePath(const std::string& path) {
    if (path.empty()) return path;

    char outPath[PLATFORM_WIN32_MAX_PATH_LENGTH] = {};
    std::string path2 = path;
    if (path2.back() != getPathSeparator()) { path2 += getPathSeparator(); }
    PathCanonicalizeA(outPath, path2.c_str());
    return std::string { outPath };
}

constexpr char h_editor::platform::getPathSeparator() {
    return '\\';
}

h_editor::platform::FileType h_editor::platform::getFileType(const std::string& path) {
    return GetFileAttributesA(path.c_str()) & FILE_ATTRIBUTE_DIRECTORY ? h_editor::platform::FileType::DIRECTORY
                                                                       : h_editor::platform::FileType::FILE;
}

std::string h_editor::platform::getFileExtension(const std::string& path) {
    auto dotIter = path.find_last_of('.');
    if (dotIter == std::string::npos) {
        return std::string {};
    }
    return path.substr(dotIter + 1);
}

std::string h_editor::platform::getDirectoryFromPath(const std::string& path) {
    return path.substr(0, path.find_last_of(getPathSeparator()) + 1);
}

std::string h_editor::platform::getBaseFromPath(const std::string& path) {
    return path.substr(0, path.find_last_of('.'));
}

std::string h_editor::platform::getJustFile(const std::string& path) {
    auto lastSlashIter = path.find_last_of(getPathSeparator());
    if (lastSlashIter == std::string::npos) {
        return path;
    }
    return path.substr(lastSlashIter);
}