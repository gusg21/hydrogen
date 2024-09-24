//
// Created by angus.goucher on 9/24/2024.
//

#include "editor/windows/sceneeditor.h"

#include <imgui.h>

#include "editor/editor.h"

void h_editor::windows::SceneEditor::open(const std::string& assetPath) {
    const char* yamlText = static_cast<char*>(SDL_LoadFile((getEditor()->getProjectBasePath() + assetPath).c_str(), nullptr));
    m_sceneSpecYaml = YAML::Load(yamlText);
    SDL_free((void*)yamlText);

    spec.initFromYaml(m_sceneSpecYaml);
}

void h_editor::windows::SceneEditor::paintContent() {
    for (uint32_t instIndex = 0; instIndex < spec.actorSpecIndices.size(); instIndex ++) {

        ImGui::InputInt("Index", reinterpret_cast<int*>(&spec.actorSpecIndices[instIndex]));

    }
}
