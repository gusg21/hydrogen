//
// Created by Angus Goucher on 9/1/2024.
//

#include "core/runtimeengine.h"
#include "imgui.h"

#include "core/systems/gravity.h"
#include "core/systems/render/renderer.h"
#include "core/systems/script/scripting.h"

void h_core::RuntimeEngine::doGUI() {
    Engine::doGUI();

    // Demo ImGui window
    if (m_showImGuiDemo) ImGui::ShowDemoWindow();

    if (ImGui::Begin("Runtime Debugger")) {
        ImGui::Checkbox("Show ImGui Demo", &m_showImGuiDemo);

        if (ImGui::CollapsingHeader("Project")) {
            ImGui::SeparatorText("Properties");

            ImGui::BeginTable("Project Info", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders);
            {
                ImGui::TableNextColumn();
                ImGui::Text("Project Name");
                ImGui::TableNextColumn();
                ImGui::Text("%s", getProject()->name.c_str());

                ImGui::TableNextColumn();
                ImGui::Text("Window Dimensions");
                ImGui::TableNextColumn();
                ImGui::Text("%ux%u", getProject()->windowWidth, getProject()->windowHeight);
            }
            ImGui::EndTable();

            // Asset List
            ImGui::SeparatorText("Assets");

            ImGui::BeginTable("Project Info", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp);
            ImGui::TableSetupScrollFreeze(3, 1);
            ImGui::TableSetupColumn("Index", 0, 20.f);
            ImGui::TableSetupColumn("Type", 0, 20.f);
            ImGui::TableSetupColumn("Path", 0, 200.f);
            ImGui::TableHeadersRow();

            for (const h_core::ProjectAssetEntry& entry : getProject()->requiredAssets) {
                ImGui::TableNextColumn();
                ImGui::Text("%u", entry.index);
                ImGui::TableNextColumn();
                ImGui::Text("%u", entry.typeId);
                ImGui::TableNextColumn();
                ImGui::Text("%s", entry.assetPath.c_str());
            }
            ImGui::EndTable();
        }
    }
    ImGui::End();
}

void h_core::RuntimeEngine::beginFrame() {
    Engine::beginFrame();

    m_systems.beginFrame();
}

void h_core::RuntimeEngine::doProcess() {
    Engine::doProcess();

    m_systems.processScene(getScene());
}

void h_core::RuntimeEngine::doDraw() {
    Engine::doDraw();

    m_systems.drawScene(getScene());
}

void h_core::RuntimeEngine::endFrame() {
    Engine::endFrame();

    m_systems.endFrame();
}
void h_core::RuntimeEngine::destroy() {
    m_systems.destroy();

    // THEN destroy the engine internals
    Engine::destroy();
}

void h_core::RuntimeEngine::doInit() {
    Engine::doInit();

    // set up systems
    m_systems.gravity = new h_core::systems::Gravity();
    m_systems.renderer = new h_core::render::Renderer();
    m_systems.scripting = new h_core::script::Scripting();
    m_systems.init(this);
}

void h_core::RuntimeEngine::doPostLoad() {
    Engine::doPostLoad();

    getAssets()->precompile(&m_systems);
}

void h_core::RuntimeEngine::prepareScene(h_core::AssetIndex sceneSpecIndex) {
    // Prepare the actors for this scene
    if (sceneSpecIndex != ASSETS_ASSET_INDEX_BAD) {
        getScene()->addActorsFromSceneSpec(
            getAssets(), sceneSpecIndex,
            m_systems.scripting->getContext());
    }

    // Prepare the systems for this scene
    m_systems.prepareScene(getScene());
}
