//
// Created by angus.goucher on 9/24/2024.
//

#include "editor/windows/sceneeditor.h"

#include <imgui.h>

#include "core/actorspecasset.h"
#include "core/log.h"
#include "editor/editor.h"

h_editor::windows::SceneEditor* h_editor::windows::SceneEditor::instance = nullptr;

void h_editor::windows::SceneEditor::open(const std::string& assetPath) {
    instance = this;

    void* yamlText = SDL_LoadFile((getEditor()->getProjectBasePath() + assetPath).c_str(), nullptr);
    m_sceneSpecYaml = YAML::Load(static_cast<char*>(yamlText));
    SDL_free(yamlText);

    spec.initFromYaml(m_sceneSpecYaml);

    for (const h_core::project::ProjectAssetEntry& requiredAsset : getEditor()->getProject()->requiredAssets) {
        for (const uint32_t actorSpecIndex : spec.actorSpecIndices) {
            if (requiredAsset.index == actorSpecIndex) {
                h_core::ActorSpecAsset actorSpec {};

                void* actorYamlText =
                    SDL_LoadFile((getEditor()->getProjectBasePath() + requiredAsset.assetPath).c_str(), nullptr);
                YAML::Node actorYaml = YAML::Load(static_cast<char*>(actorYamlText));
                SDL_free(actorYamlText);

                actorSpec.initFromYaml(actorYaml);
                actorSpecs.push_back(actorSpec);
            }
        }
    }
}

void h_editor::windows::SceneEditor::close() {
    AssetEditorWindow::close();
    instance = nullptr;
}

void h_editor::windows::SceneEditor::paintContent() {
    // for (uint32_t instIndex = 0; instIndex < spec.actorSpecIndices.size(); instIndex ++) {
    //     ImGui::InputInt("Index", reinterpret_cast<int*>(&spec.actorSpecIndices[instIndex]));
    // }

    // Hacky solution, currently just for testing
    for (uint32_t specIndex = 0; specIndex < actorSpecs.size(); specIndex++) {
        ImGui::PushID(&actorSpecs[specIndex]);
        ImGui::Text("Mask %d", actorSpecs[specIndex].mask);
        ImGui::DragFloat3("Position", &actorSpecs[specIndex].transform.position.x);
        ImGui::PopID();
    }

    // TODO: only accept if the object is a hyactor (or our other defined stuffs for putting into the scene)
    // TODO: figure out why it isn't getting the drop target, different windows?
    if (ImGui::BeginDragDropTarget()) {
        HYLOG_INFO("SCENE EDITOR: received drop info...");
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Actor")) {
            HYLOG_INFO("SCENE EDITOR: accepted a payload");
        }

        ImGui::EndDragDropSource();
    }
}

void h_editor::windows::SceneEditor::addActor(h_core::ActorSpecAsset actorSpec) {
    actorSpecs.push_back(actorSpec);
}

void h_editor::windows::SceneEditor::saveToYaml() {

}