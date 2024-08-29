#include <stdio.h>

#include "core/actorspecasset.h"
#include "core/scene.h"
#include "core/system.h"

void h_core::Scene::initFromSceneSpecAssetIndex(
    h_core::Assets* assets, h_core::AssetIndex sceneSpecIndex,
    asIScriptContext* scriptingContext) {
    h_core::SceneSpecAsset* sceneSpec =
        assets->getAssetByIndex<h_core::SceneSpecAsset>(sceneSpecIndex);

    for (uint32_t actorSpecIndex = 0;
         actorSpecIndex < sceneSpec->actorSpecIndices.size();
         actorSpecIndex++) {
        addActor(
            assets->getAssetByIndex<h_core::ActorSpecAsset>(
                sceneSpec->actorSpecIndices[actorSpecIndex]),
            scriptingContext);
    }
}

h_core::ActorId h_core::Scene::addActor(
    ActorSpecAsset* spec, asIScriptContext* scriptingContext) {
    ActorId newId = m_nextId;
    masks[newId] = spec->mask;
    transforms[newId] = spec->transform;
    meshes[newId] = spec->mesh;
    scripts[newId].init(spec->script, scriptingContext, newId);
    ::printf("INFO: SCENE: adding actor id %d, mask %d\n", newId, masks[newId]);
    m_nextId++;
    return newId;
}
