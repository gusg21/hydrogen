//
// Created by angus.goucher on 9/24/2024.
//

#include "editor/windows/sceneeditor.h"

#include <core/theming/guicolors.h>
#include <imgui.h>

#include "core/actorspecasset.h"
#include "core/input/dualkeyinputactionsource.h"
#include "core/input/input.h"
#include "core/input/keyinputactionsource.h"
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

    m_spec.initFromYaml(getEditor()->getProjectBasePath(), m_sceneSpecYaml);

    for (const h_core::project::ProjectAssetEntry& requiredAsset : getEditor()->getProject()->requiredAssets) {
        for (const uint32_t actorSpecIndex : m_spec.actorSpecIndices) {
            if (requiredAsset.index == actorSpecIndex) {
                EditorActorSpec actorSpec {};

                void* actorYamlText =
                    SDL_LoadFile((getEditor()->getProjectBasePath() + requiredAsset.assetPath).c_str(), nullptr);
                YAML::Node actorYaml = YAML::Load(static_cast<char*>(actorYamlText));
                SDL_free(actorYamlText);

                actorSpec.actorSpec.initFromYaml(getEditor()->getProjectBasePath(), actorYaml);
                actorSpec.index = actorSpecIndex;
                m_actorSpecs.push_back(actorSpec);
            }
        }
    }

    // Inputs
    m_camForwardInputIndex = getEditor()->getInput()->newAction("cam_forward");
    getEditor()
        ->getInput()
        ->getAction(m_camForwardInputIndex)
        ->sources.push_back(new h_core::input::DualKeyInputActionSource(SDL_SCANCODE_W, SDL_SCANCODE_S));

    m_camRightInputIndex = getEditor()->getInput()->newAction("cam_right");
    getEditor()
        ->getInput()
        ->getAction(m_camRightInputIndex)
        ->sources.push_back(new h_core::input::DualKeyInputActionSource(SDL_SCANCODE_D, SDL_SCANCODE_A));

    m_reclaimMouseInputIndex = getEditor()->getInput()->newAction("reclaim_mouse");
    getEditor()
        ->getInput()
        ->getAction(m_reclaimMouseInputIndex)
        ->sources.push_back(new h_core::input::KeyInputActionSource(SDL_SCANCODE_ESCAPE));
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
        ImGui::SeparatorText("Renderer");
        ImGui::DragFloat("FOV", &m_renderer->fovY, 1, 30.f, 160.f);
        ImGui::DragFloat3("Position", &m_renderer->cameraPosition.x);
        ImGui::DragFloat3("Direction", &m_renderer->cameraDirection.x);

        ImGui::SeparatorText("FlyCam");
        ImGui::DragFloat("Sensitivity", &m_flyCamSensitivity);
        ImGui::DragFloat("Speed", &m_flyCamSpeed);
        ImGui::Separator();

        ImGui::SeparatorText("Actors");
        std::vector<uint32_t> specsToRemove {};
        for (uint32_t specIndex = 0; specIndex < m_actorSpecs.size(); specIndex++) {
            ImGui::PushID(static_cast<int>(m_actorSpecs[specIndex].index));
            {
                if (ImGui::SmallButton("-")) {
                    specsToRemove.push_back(specIndex);
                }
                ImGui::Text("Actor Spec Index: %d", m_actorSpecs[specIndex].index);
                ImGui::Text("Mask: %d", m_actorSpecs[specIndex].actorSpec.mask);
                ImGui::DragFloat3("Position", &m_actorSpecs[specIndex].actorSpec.transform.position.x);
                ImGui::DragFloat4("Rotation", &m_actorSpecs[specIndex].actorSpec.transform.rotation.x);
                ImGui::DragFloat3("Scale", &m_actorSpecs[specIndex].actorSpec.transform.scale.x);
            }
            ImGui::PopID();

            ImGui::Separator();
        }

        for (uint32_t spec : specsToRemove) {
            m_actorSpecs.erase(m_actorSpecs.begin() + spec);
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Cam controls
    h_core::input::Input* input = getEditor()->getInput();
    double delta = getEditor()->getDeltaSecs();

    // Viewport
    if (ImGui::BeginChild("Scene Viewport Container", ImVec2 { -0.1, -0.1f }, 0, ImGuiWindowFlags_NoScrollbar)) {
        bool viewportBorder = input->mouseCaptured;
        if (viewportBorder) { ImGui::PushStyleColor(ImGuiCol_Border, IMGUI_COLOR_WARN); }

        if (ImGui::BeginChild("Scene Viewport", ImVec2 { 810, 610 }, ImGuiChildFlags_Border)) {
            m_renderer->render(this);
            ImGui::SetCursorPos(ImVec2 { 5, 5 });
            ImGui::Image(m_renderer->getTexture(), ImVec2 { 800, 600 }, ImVec2 { 0, 1 }, ImVec2 { 1, 0 });
        }
        ImGui::EndChild();

        // Toggle mouse capture
        if (!input->mouseCaptured && ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0)) {
            // Clicked on viewport
            input->mouseCaptured = true;
            m_mouseGrabPos = { input->getMouseX(), input->getMouseY() };
        }
        else if (input->mouseCaptured && input->getDigitalPressed(m_reclaimMouseInputIndex)) {
            input->mouseCaptured = false;
        }

        if (viewportBorder) { ImGui::PopStyleColor(); }

        ImGui::TextDisabled("Double-click to FlyCam");
    }
    ImGui::EndChild();

    // Mouse looking (if captured)
    if (input->mouseCaptured) {
        m_flyCamYaw += input->getMouseDeltaX() * m_flyCamSensitivity * delta;
        m_flyCamPitch -= input->getMouseDeltaY() * m_flyCamSensitivity * delta;
        m_flyCamPitch = MATH_CLAMP(m_flyCamPitch, -89.0f, 89.0f);

        // Calculate camera orientation space
        float yawRad = (m_flyCamYaw / 360.f) * MATH_TAU;
        float pitchRad = (m_flyCamPitch / 360.f) * MATH_TAU;
        h_core::math::Vector3 forward {};
        forward.x = ::cosf(pitchRad) * ::sinf(yawRad);
        forward.y = ::sinf(pitchRad);
        forward.z = ::cosf(pitchRad) * -::cosf(yawRad);
        forward = h_core::math::Vector3::normalize(forward);
        h_core::math::Vector3 right = h_core::math::Vector3::normalize(
            h_core::math::Vector3::cross(forward, h_core::math::Vector3 { 0.f, 1.f, 0.f }));
        m_renderer->cameraDirection = forward;

        // Move along axes in cam orientation space
        float forwardAmount = input->getAnalogValue(m_camForwardInputIndex);
        m_renderer->cameraPosition += forward * forwardAmount * delta * m_flyCamSpeed;

        float rightAmount = input->getAnalogValue(m_camRightInputIndex);
        m_renderer->cameraPosition += right * rightAmount * delta * m_flyCamSpeed;

        // Grab mouse fully
        input->setMousePos(m_mouseGrabPos.x, m_mouseGrabPos.y);
        ImGui::GetIO().MouseClicked[0] = false;
        ImGui::GetIO().MousePos = ImVec2 { m_mouseGrabPos.x, m_mouseGrabPos.y };
    }
}

void h_editor::windows::SceneEditor::addActor(const YAML::Node& yaml, h_core::AssetIndex index) {
    h_core::ActorSpecAsset actorSpec {};
    actorSpec.initFromYaml(getEditor()->getProjectBasePath(), yaml);
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

    if (path.empty()) {
        // Don't try to load remote assets
        return nullptr;
    }

    // Load if unloaded
    if (m_cachedModels.find(path) == m_cachedModels.end()) {
        h_core::render::ModelAsset* model = new h_core::render::ModelAsset();
        char* yamlText = (char*)SDL_LoadFile((getEditor()->getProjectBasePath() + path).c_str(), nullptr);
        YAML::Node yaml = YAML::Load(yamlText);
        model->initFromYaml(getEditor()->getProjectBasePath(), yaml);
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