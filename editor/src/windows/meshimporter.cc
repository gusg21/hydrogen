//
// Created by angus.goucher on 9/13/2024.
//

#include "editor/windows/meshimporter.h"

#include <imgui.h>

#include "editor/platform/path.h"
#include "editor/platform/environment.h"
#include "editor/editor.h"
#include "core/log.h"

void h_editor::windows::MeshImporter::open(const std::string& meshFile) {
    this->m_meshFile = meshFile;

    std::string extension = h_editor::platform::getFileExtension(meshFile);
    std::string directory = h_editor::platform::getDirectoryFromPath(meshFile);

    std::string fbx2GltfPath = h_editor::platform::getEnvironmentVar("FBX2GLTF");
    HYLOG_DEBUG("FBX2GLTF: %s", fbx2GltfPath.c_str());
    if (!fbx2GltfPath.empty()) {
        memcpy_s(m_fbx2gltfEntry, MESHIMPORTER_ENTRY_LENGTH, fbx2GltfPath.c_str(), fbx2GltfPath.size());
    }

    memcpy_s(m_outputPathEntry, MESHIMPORTER_ENTRY_LENGTH, directory.c_str(), directory.size());

    if (extension == "fbx") {
        openFbx();
    } else if (extension == "glb") {
        openGltf(true);
    } else if (extension == "gltf") {
        openGltf(false);
    } else {
        // Unknown file type
        getEditor()->openModal("Unknown model file type " + extension);
        close();
    }
}

void h_editor::windows::MeshImporter::paintContent() {
    // EditorWindow::paintContent();

    ImGui::Text("Importing %s", m_meshFile.c_str());

    ImGui::Separator();

    if (m_needsFbx2Gltf) {
        ImGui::InputText("fbx2gltf", m_fbx2gltfEntry, MESHIMPORTER_ENTRY_LENGTH);
        ImGui::TextDisabled("Hint: Set the environment variable FBX2GLTF to populate the above automatically.");
    }

    if (m_needsBasePath) {
        ImGui::InputText("Model Base Directory", m_basePathEntry, MESHIMPORTER_ENTRY_LENGTH);
    }

    ImGui::InputText("Output File", m_outputPathEntry, MESHIMPORTER_ENTRY_LENGTH);

    ImGui::Separator();

    doImportAndCloseButtons();
}

void h_editor::windows::MeshImporter::openFbx() {
    m_needsFbx2Gltf = true;
}

void h_editor::windows::MeshImporter::openGltf(bool binary) {
    m_needsBasePath = binary;
}

void h_editor::windows::MeshImporter::import() {
    HYLOG_DEBUG("Importing!");
}
