#include <stdio.h>

#include "SDL.h"
#include "imgui.h"

#include "core/actorspecasset.h"
#include "core/log.h"
#include "core/scene.h"
#include "core/theming/guicolors.h"
#include "core/widgets.h"

void h_core::Scene::addActorsFromSceneSpec(
    h_core::Assets* assets, h_core::AssetIndex sceneSpecIndex, asIScriptContext* scriptingContext) {
    h_core::SceneSpecAsset* sceneSpec = assets->getAssetByIndex<h_core::SceneSpecAsset>(sceneSpecIndex);

    for (uint32_t actorSpecIndex = 0; actorSpecIndex < sceneSpec->actorSpecIndices.size(); actorSpecIndex++) {
        addActor(assets, sceneSpec->actorSpecIndices[actorSpecIndex], scriptingContext);
    }
}

h_core::ActorId h_core::Scene::addActor(
    h_core::Assets* assets, h_core::AssetIndex actorSpecIndex, asIScriptContext* scriptingContext) {
    h_core::ActorSpecAsset* spec = assets->getAssetByIndex<h_core::ActorSpecAsset>(actorSpecIndex);

    if (m_nextId >= SCENE_MAX_ACTORS) {
        HYLOG_ERROR("SCENE: Too many actors!\n");
        return UINT32_MAX;
    }

    ActorId newId = m_nextId;
    masks[newId] = spec->mask;
    transforms[newId] = spec->transform;
    rigidbodies[newId] = spec->rigidbody;

    // components
    meshes[newId].init(spec->meshIndex);
    scripts[newId].init(spec->scriptIndex, assets, scriptingContext, newId);

    HYLOG_INFO("SCENE: adding actor id %d, mask %d\n", newId, masks[newId]);

    m_nextId++;
    m_numActors++;

    return newId;
}

void h_core::Scene::doGUI(h_core::Assets* assets) {
    if (ImGui::Begin("Scene Debugger")) {
        ImGui::Text("Actors: %zu/%d", m_numActors, SCENE_MAX_ACTORS);

        for (h_core::ActorId id = 0; id < SCENE_MAX_ACTORS; id++) {
            if (masks[id] != 0) {
                ImGui::PushID((int)id);
                if (ImGui::CollapsingHeader("Actor")) {
                    ImGui::Indent();
                    ImGui::Text("ID: %d", id);
                    ImGui::Text("Component Bitfield: %d", masks[id]);
                    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                        ImGui::Indent();
                        ImGui::DragFloat3("Position", &transforms[id].position.x);
                        ImGui::DragFloat4("Rotation", &transforms[id].rotation.x);
                        ImGui::DragFloat3("Scale", &transforms[id].scale.x);
                        ImGui::Unindent();
                    }
                    if (masks[id] & h_core::render::MeshComp::getMask()) {
                        if (ImGui::CollapsingHeader("Mesh")) {
                            h_core::render::MeshAsset* mesh =
                                assets->getAssetByIndex<h_core::render::MeshAsset>(meshes[id].mesh);

                            ImGui::Indent();
                            ImGui::Text("Mesh Asset: %d", meshes[id].mesh);

                            if (mesh != nullptr) {
                                ImGui::Text("Vertex Count: %zu", mesh->getNumVertices());
                                ImGui::Text("Index Count: %zu", mesh->getNumIndices());
                            }
                            else { ImGui::TextColored(IMGUI_COLOR_WARN, "Mesh Asset not loaded"); }

                            ImGui::Unindent();
                        }
                    }
                    if (masks[id] & h_core::script::ScriptComp::getMask()) {
                        if (ImGui::CollapsingHeader("Script")) {
                            h_core::script::ScriptAsset* script =
                                assets->getAssetByIndex<h_core::script::ScriptAsset>(scripts[id].scriptAsset);

                            ImGui::Indent();
                            ImGui::Text("Script Asset: %d", scripts[id].scriptAsset);

                            if (script != nullptr) {
                                ImGui::Text("Script Class Name: %s", script->type->GetName());
                                ImGui::Text("Name: %s", script->name.c_str());
                                if (ImGui::CollapsingHeader("Code")) {
                                    if (ImGui::BeginChild("Code View")) { ImGui::Text("%s", script->code.c_str()); }
                                    ImGui::EndChild();
                                }
                            }
                            else { ImGui::TextColored(IMGUI_COLOR_WARN, "Script Asset not loaded"); }

                            ImGui::Unindent();
                        }
                    }
                    ImGui::Unindent();
                }
                ImGui::PopID();
            }
        }
    }
    ImGui::End();
}
