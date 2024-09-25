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
    if (instance != nullptr) {
        close();
        return;
    }

    instance = this;
    m_assetPath = assetPath;
    m_renderer = new visuals::SceneSpecRenderer();
    m_renderer->init(getEditor());

    void* yamlText = SDL_LoadFile((getEditor()->getProjectBasePath() + assetPath).c_str(), nullptr);
    m_sceneSpecYaml = YAML::Load(static_cast<char*>(yamlText));
    SDL_free(yamlText);

    m_spec.initFromYaml(m_sceneSpecYaml);

    for (const h_core::project::ProjectAssetEntry& requiredAsset : getEditor()->getProject()->requiredAssets) {
        for (const uint32_t actorSpecIndex : m_spec.actorSpecIndices) {
            if (requiredAsset.index == actorSpecIndex) {
                EditorActorSpec actorSpec {};

                void* actorYamlText =
                    SDL_LoadFile((getEditor()->getProjectBasePath() + requiredAsset.assetPath).c_str(), nullptr);
                YAML::Node actorYaml = YAML::Load(static_cast<char*>(actorYamlText));
                SDL_free(actorYamlText);

                actorSpec.actorSpec.initFromYaml(actorYaml);
                actorSpec.index = actorSpecIndex;
                m_actorSpecs.push_back(actorSpec);
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

    ImVec2 size = ImGui::GetWindowSize();

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save")) { saveToYaml(); }
            if (ImGui::MenuItem("Quit")) { close(); }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Hacky solution, currently just for testing
    if (ImGui::BeginChild("Scene Text Editor", ImVec2 { 200, -0.1f })) {
        ImGui::Text("Window Size: %.2fx%.2f", size.x, size.y);
        ImGui::DragFloat("FOV", &m_renderer->fovY, 1, 30.f, 160.f);
        ImGui::Separator();

        for (uint32_t specIndex = 0; specIndex < m_actorSpecs.size(); specIndex++) {
            ImGui::PushID(static_cast<int>(m_actorSpecs[specIndex].index));
            ImGui::Text("Actor Spec Index: %d", m_actorSpecs[specIndex].index);
            ImGui::Text("Mask: %d", m_actorSpecs[specIndex].actorSpec.mask);
            ImGui::DragFloat3("Position", &m_actorSpecs[specIndex].actorSpec.transform.position.x);
            ImGui::PopID();

            ImGui::Separator();
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    if (ImGui::BeginChild("Scene Viewport", ImVec2 { 810, 610 })) {
        m_renderer->render(this);
        ImGui::SetCursorPos(ImVec2 { 5, 5 });
        ImGui::Image(m_renderer->getTexture(), ImVec2 { 800, 600 }, ImVec2 { 0, 1 }, ImVec2 { 1, 0 });
    }
    ImGui::EndChild();

    /*// TODO: only accept if the object is a hyactor (or our other defined stuffs for putting into the scene)
    // TODO: figure out why it isn't getting the drop target, different windows?
    if (ImGui::BeginDragDropTarget()) {
        HYLOG_INFO("SCENE EDITOR: received drop info...");
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Actor")) {
            HYLOG_INFO("SCENE EDITOR: accepted a payload");
        }

        ImGui::EndDragDropSource();
    }*/
}

void h_editor::windows::SceneEditor::addActor(const YAML::Node& yaml, h_core::AssetIndex index) {
    h_core::ActorSpecAsset actorSpec {};
    actorSpec.initFromYaml(yaml);
    m_actorSpecs.emplace_back(actorSpec, index);
    m_spec.actorSpecIndices.push_back(index);
}

uint32_t h_editor::windows::SceneEditor::getActorCount() const {
    return m_actorSpecs.size();
}

h_core::ActorId h_editor::windows::SceneEditor::getActorIdAtIndex(uint32_t index) const {
    return m_spec.actorSpecIndices[index];
}

h_core::render::ModelAsset* h_editor::windows::SceneEditor::getActorModelAtIndex(uint32_t index) {
    EditorActorSpec actorSpec = m_actorSpecs[index];
    std::string path = getEditor()->getProject()->getPathByIndex(actorSpec.actorSpec.meshIndex);

    // Load if unloaded
    if (m_cachedModels.find(path) == m_cachedModels.end()) {
        h_core::render::ModelAsset* model = new h_core::render::ModelAsset();
        YAML::Node yaml = YAML::Load(path);
        model->initFromYaml(yaml);
        m_cachedModels[path] = model;
    }

    return m_cachedModels[path];
}
h_core::Transform h_editor::windows::SceneEditor::getActorTransformAtIndex(uint32_t index) {
    return m_actorSpecs[index].actorSpec.transform;
}
bool h_editor::windows::SceneEditor::actorHasModel(uint32_t index) {
    EditorActorSpec actorSpec = m_actorSpecs[index];
    return actorSpec.actorSpec.meshIndex != ASSET_INDEX_BAD;
}

void h_editor::windows::SceneEditor::saveToYaml() {
    for (EditorActorSpec& actorSpec : m_actorSpecs) {
        // Create YAML file
        std::string path = getEditor()->getProject()->getPathByIndex(actorSpec.index);
        YAML::Node yaml;

        actorSpec.actorSpec.saveToYaml(yaml);

        SDL_RWops* yamlFile = SDL_RWFromFile((getEditor()->getProjectBasePath() + path).c_str(), "w+");
        std::string outputYaml = YAML::Dump(yaml);
        yamlFile->write(yamlFile, outputYaml.c_str(), sizeof(char), outputYaml.size());
        SDL_RWclose(yamlFile);
    }

    // Create YAML file
    YAML::Node yaml;

    m_spec.saveToYaml(yaml);

    SDL_RWops* yamlFile = SDL_RWFromFile((getEditor()->getProjectBasePath() + m_assetPath).c_str(), "w+");
    std::string outputYaml = YAML::Dump(yaml);
    yamlFile->write(yamlFile, outputYaml.c_str(), sizeof(char), outputYaml.size());
    SDL_RWclose(yamlFile);
}