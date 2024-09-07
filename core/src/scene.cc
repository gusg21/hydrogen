#include <stdio.h>

#include "SDL.h"
#include "imgui.h"

#include "core/actorspecasset.h"
#include "core/scene.h"
#include "core/system.h"

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

    ActorId newId = m_nextId;
    masks[newId] = spec->mask;
    transforms[newId] = spec->transform;

    // components
    meshes[newId].init(assets->getAssetByIndex<h_core::render::MeshAsset>(spec->meshIndex));
    scripts[newId].init(
        assets->getAssetByIndex<h_core::script::ScriptAsset>(spec->scriptIndex), scriptingContext, newId);

    ::SDL_Log("INFO: SCENE: adding actor id %d, mask %d\n", newId, masks[newId]);

    m_nextId++;
    m_numActors++;

    if (m_nextId >= SCENE_MAX_ACTORS) { ::SDL_Log("ERROR: SCENE: Too many actors!\n"); }

    return newId;
}

void h_core::Scene::doGUI() {
    if (ImGui::Begin("Scene Debugger")) {
        ImGui::Text("Actors: %zu/%d", m_numActors, SCENE_MAX_ACTORS);

        for (h_core::ActorId id = 0; id < SCENE_MAX_ACTORS; id++) {
            if (masks[id] != 0) {
                ImGui::PushID(id);
                if (ImGui::CollapsingHeader("Actor")) {
                    ImGui::Indent();
                    ImGui::Text("ID: %d", id);
                    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                        ImGui::Indent();
                        ImGui::DragFloat3("Position", &transforms[id].position.x);
                        ImGui::DragFloat4("Rotation", &transforms[id].rotation.x);
                        ImGui::DragFloat3("Scale", &transforms[id].scale.x);
                        ImGui::Unindent();
                    }
                    if (ImGui::CollapsingHeader("Mesh")) {
                        ImGui::Indent();
                        ImGui::Text("Mesh Asset: %p", meshes[id].mesh);
                        ImGui::Text("Vertex Count: %zu", meshes[id].mesh->getNumVertices());
                        ImGui::Text("Index Count: %zu", meshes[id].mesh->getNumIndices());
                        ImGui::Unindent();
                    }
                    if (ImGui::CollapsingHeader("Script")) {
                        ImGui::Indent();
                        ImGui::Text("Script Asset: %p", scripts[id].scriptAsset);
                        ImGui::Text("Script Class Name: %s", scripts[id].scriptAsset->type->GetName());
                        ImGui::Text("Name: %s", scripts[id].scriptAsset->name.c_str());
                        if (ImGui::CollapsingHeader("Code")) {
                            if (ImGui::BeginChild("Code View")) {
                                ImGui::Text("%s", scripts[id].scriptAsset->code.c_str());
                            }
                            ImGui::EndChild();
                        }
                        ImGui::Unindent();
                    }
                    ImGui::Unindent();
                }
                ImGui::PopID();
            }
        }
    }
    ImGui::End();
}
