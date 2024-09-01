#include <stdio.h>

#include "core/actorspecasset.h"
#include "core/scene.h"
#include "core/system.h"

void h_core::Scene::addActorsFromSceneSpec(
    h_core::Assets* assets, h_core::AssetIndex sceneSpecIndex,
    asIScriptContext* scriptingContext) {
    h_core::SceneSpecAsset* sceneSpec =
        assets->getAssetByIndex<h_core::SceneSpecAsset>(sceneSpecIndex);

    for (uint32_t actorSpecIndex = 0;
         actorSpecIndex < sceneSpec->actorSpecIndices.size();
         actorSpecIndex++) {
        addActor(
            assets, sceneSpec->actorSpecIndices[actorSpecIndex],
            scriptingContext);
    }
}

h_core::ActorId h_core::Scene::addActor(
    h_core::Assets* assets, h_core::AssetIndex actorSpecIndex,
    asIScriptContext* scriptingContext) {
    h_core::ActorSpecAsset* spec =
        assets->getAssetByIndex<h_core::ActorSpecAsset>(actorSpecIndex);

    ActorId newId = m_nextId;
    masks[newId] = spec->mask;
    transforms[newId] = spec->transform;

    // components
    meshes[newId].init(
        assets->getAssetByIndex<h_core::render::MeshAsset>(spec->meshIndex));
    scripts[newId].init(
        assets->getAssetByIndex<h_core::script::ScriptAsset>(spec->scriptIndex),
        scriptingContext, newId);

    ::printf("INFO: SCENE: adding actor id %d, mask %d\n", newId, masks[newId]);

    m_nextId++;
    return newId;
}
