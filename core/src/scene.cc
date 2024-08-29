#include <stdio.h>

#include "core/actorspec.h"
#include "core/scene.h"
#include "core/system.h"

void h_core::Scene::initFromSceneSpecAssetIndex(
    h_core::Assets* assets, h_core::AssetIndex sceneSpecIndex,
    asIScriptContext* scriptingContext) {
    h_core::SceneSpec* sceneSpec =
        assets->getAssetByIndex<h_core::SceneSpec>(sceneSpecIndex);

    for (uint32_t actorSpecIndex = 0;
         actorSpecIndex < sceneSpec->actorSpecIndices.size();
         actorSpecIndex++) {
        addActor(
            assets->getAssetByIndex<h_core::ActorSpec>(
                sceneSpec->actorSpecIndices[actorSpecIndex]),
            scriptingContext);
    }
}

h_core::ActorId h_core::Scene::addActor(
    ActorSpec* spec, asIScriptContext* scriptingContext) {
    ActorId newId = m_nextId;
    masks[newId] = spec->mask;
    transforms[newId] = spec->transform;
    meshes[newId] = spec->mesh;
    scripts[newId].init(spec->script, scriptingContext, newId);
    ::printf("INFO: SCENE: adding actor id %d, mask %d\n", newId, masks[newId]);
    m_nextId++;
    return newId;
}
