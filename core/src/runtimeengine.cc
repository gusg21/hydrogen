//
// Created by Angus Goucher on 9/1/2024.
//

#include "core/runtimeengine.h"
#include "imgui.h"

#include "core/input/dualkeyinputactionsource.h"
#include "core/input/keyinputactionsource.h"
#include "core/systems/gravity.h"
#include "core/systems/render/gles3renderer.h"
#include "core/systems/render/gl4renderer.h"
#include "core/systems/script/scripting.h"

void h_core::RuntimeEngine::doInit() {
    Engine::doInit();

    // set up systems
    m_systems.gravity = new h_core::systems::Gravity();
    if (getWindow()->isGles3()) {
        m_systems.renderer = new h_core::render::Gles3Renderer();
    }
    else {
        m_systems.renderer = new h_core::render::Gles3Renderer();
    }
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
        getScene()->addActorsFromSceneSpec(getAssets(), sceneSpecIndex, m_systems.scripting->getContext());
    }

    // Prepare the systems for this scene
    m_systems.prepareScene(getScene());
}


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

            ImGui::BeginTable(
                "Project Info", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp);
            ImGui::TableSetupScrollFreeze(3, 1);
            ImGui::TableSetupColumn("Index", 0, 20.f);
            ImGui::TableSetupColumn("Type", 0, 20.f);
            ImGui::TableSetupColumn("Path", 0, 200.f);
            ImGui::TableHeadersRow();

            for (const h_core::project::ProjectAssetEntry& entry : getProject()->requiredAssets) {
                ImGui::TableNextColumn();
                ImGui::Text("%u", entry.index);
                ImGui::TableNextColumn();
                ImGui::Text("%u", entry.typeId);
                ImGui::TableNextColumn();
                ImGui::Text("%s", entry.assetPath.c_str());
            }
            ImGui::EndTable();
        }

        ImGui::SeparatorText("Statistics");
        ImGui::Text("FPS: %.3f", getFPS());
        ImGui::Text("Avg FPS: %.3f", m_averageFPS);
        ImGui::Text("Frame Time: %.3fms", getDeltaSecs() * 1000.0);
    }
    ImGui::End();

    m_systems.doGUI();
    getInput()->doGUI();
    getScene()->doGUI();
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

    m_fpsSamples.push_back(getFPS());
    if (m_fpsSamples.size() > RUNTIMEENGINE_MAX_FPS_SAMPLES) { m_fpsSamples.pop_front(); }
    m_averageFPS = 0;
    for (uint32_t sampleIndex = 0; sampleIndex < m_fpsSamples.size(); sampleIndex++) {
        m_averageFPS += m_fpsSamples[sampleIndex];
    }
    m_averageFPS = m_averageFPS / (double)m_fpsSamples.size();
}

void h_core::RuntimeEngine::destroy() {
    m_systems.destroy();

    // THEN destroy the engine internals
    Engine::destroy();
}
