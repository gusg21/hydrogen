//
// Created by angus.goucher on 9/13/2024.
//

#include "editor/windows/meshimporter.h"

#include <imgui.h>

#include "core/log.h"
#include "editor/editor.h"
#include "editor/platform/environment.h"
#include "editor/platform/invoke.h"
#include "editor/platform/path.h"

void h_editor::windows::MeshImporter::open(const std::string& meshFile) {
    this->m_meshFile = meshFile;

    std::string extension = h_editor::platform::getFileExtension(meshFile);
    std::string outputDefault = h_editor::platform::getBaseFromPath(meshFile) + ".hymodel";

    std::string fbx2GltfPath = h_editor::platform::getEnvironmentVar("FBX2GLTF");
    HYLOG_DEBUG("FBX2GLTF: %s", fbx2GltfPath.c_str());
    if (!fbx2GltfPath.empty()) {
        memcpy_s(m_fbx2gltfEntry, MESHIMPORTER_ENTRY_LENGTH, fbx2GltfPath.c_str(), fbx2GltfPath.size());
    }

    memcpy_s(m_outputPathEntry, MESHIMPORTER_ENTRY_LENGTH, outputDefault.c_str(), outputDefault.size());

    if (extension == "fbx") { openFbx(); }
    else if (extension == "glb") { openGltf(true); }
    else if (extension == "gltf") { openGltf(false); }
    else {
        // Unknown file type
        getEditor()->openModal("Unknown model file type " + extension);
        close();
    }
}

void h_editor::windows::MeshImporter::paintContent() {
    // EditorWindow::paintContent();

    ImGui::Text("Importing %s", m_meshFile.c_str());
    if (m_needsFbx2Gltf) ImGui::TextDisabled("Computed GLB path: %s", getGlbPath().c_str());

    ImGui::Separator();

    if (m_needsFbx2Gltf) {
        ImGui::InputText("fbx2gltf", m_fbx2gltfEntry, MESHIMPORTER_ENTRY_LENGTH);
        ImGui::TextDisabled("Hint: Set the environment variable FBX2GLTF to populate the above automatically.");
    }

    if (m_needsBasePath) {
        ImGui::BeginDisabled();
        std::string basePath = h_editor::platform::getBaseFromPath(m_meshFile) + '\\';
        ImGui::InputText("Model Base Directory", const_cast<char*>(basePath.c_str()), MESHIMPORTER_ENTRY_LENGTH);
        ImGui::EndDisabled();
    }

    ImGui::BeginDisabled();
    bool always = true;
    ImGui::Checkbox("Output Binary (.glb)", &always);
    ImGui::EndDisabled();

    ImGui::InputText("Output File", m_outputPathEntry, MESHIMPORTER_ENTRY_LENGTH);

    ImGui::Separator();

    doImportAndCloseButtons();
}

void h_editor::windows::MeshImporter::openFbx() {
    m_needsFbx2Gltf = true;
}

void h_editor::windows::MeshImporter::openGltf(bool binary) {
    m_needsBasePath = !binary;
}

std::string h_editor::windows::MeshImporter::getGlbPath() {
    return h_editor::platform::getBaseFromPath(m_meshFile) + ".glb";
}

void h_editor::windows::MeshImporter::import() {
    HYLOG_DEBUG("Importing!");

    // Convert to gltf/glb
    std::string gltfFile;
    if (m_needsFbx2Gltf) {
        // Call FBX2GLTF
        std::string args = "--binary --input \"" + getEditor()->getProjectBasePath() + m_meshFile + "\" --output \"" + getEditor()->getProjectBasePath() + getGlbPath() + "\"";
        char* argsCStr = static_cast<char*>(calloc(args.size() + 1, sizeof(char)));
        memcpy(argsCStr, args.c_str(), args.size());

        uint32_t result = h_editor::platform::invokeBlocking(m_fbx2gltfEntry, argsCStr);
        if (result != 0) {
            getEditor()->openModal("Failed to call FBX2GLTF, error code: " + std::to_string(result));
            return;
        }

        gltfFile = getGlbPath();

        delete argsCStr;
    }
    else { gltfFile = m_meshFile; }

    // Convert to HYMODEL

    // Create YAML
    YAML::Node yaml {};
    yaml["gltf"] = gltfFile;
    if (m_needsBasePath) {
        yaml["gltf_base_path"] = h_editor::platform::getBaseFromPath(m_meshFile) + '\\';
    } else {
        yaml["gltf_binary"] = true;
    }

    // Create YAML file
    std::string outputYamlPath = m_outputPathEntry;
    SDL_RWops* yamlFile = SDL_RWFromFile((getEditor()->getProjectBasePath() + outputYamlPath).c_str(), "w+");
    std::string outputYaml = YAML::Dump(yaml);
    yamlFile->write(yamlFile, outputYaml.c_str(), sizeof(char), outputYaml.size());
    SDL_RWclose(yamlFile);
}

void h_editor::windows::MeshImporter::openAndImportDefault(const std::string& meshFile) {
    open(meshFile);
    import();
    close();
}

void h_editor::windows::MeshImporter::openAndImportToPath(
    const std::string& meshFile, const std::string& outputAssetFile) {
    open(meshFile);
    // Set the output path
    memcpy(m_outputPathEntry, outputAssetFile.c_str(), MIN(outputAssetFile.size(), MESHIMPORTER_ENTRY_LENGTH));
    import();
    close();
}
