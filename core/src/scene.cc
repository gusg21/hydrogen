#pragma once

#include <stdio.h>

#include "core/actorspec.h"
#include "core/scene.h"
#include "core/system.h"

void h_core::Scene::initFromSceneSpecAssetIndex(
    h_core::Assets* assets, h_core::AssetIndex sceneSpecIndex) {
    h_core::SceneSpec* sceneSpec =
        assets->getAssetByIndex<h_core::SceneSpec>(sceneSpecIndex);

    for (uint32_t actorSpecIndex = 0;
         actorSpecIndex < sceneSpec->actorSpecIndices.size(); actorSpecIndex++) {
        addActor(assets->getAssetByIndex<h_core::ActorSpec>(
            sceneSpec->actorSpecIndices[actorSpecIndex]));
    }
}

h_core::ActorId h_core::Scene::addActor(ActorSpec* spec) {
    ActorId newId = m_nextId;
    m_masks[newId] = spec->mask;
    m_transforms[newId] = spec->transform;
    printf("INFO: SCENE: adding actor id %d, mask %d\n", newId, m_masks[newId]);
    m_nextId++;
    return newId;
}

void h_core::Scene::updateSystemReferences(
    h_core::System* system, h_core::ActorId id) {
    system->transform = &m_transforms[id];
    system->mesh = &m_meshes[id];
}

void h_core::Scene::processSystem(h_core::System* system) {
    ComponentBitmask requiredMask = system->getMask();

    for (ActorId id = 0; id < SCENE_MAX_ACTORS; id++) {
        ComponentBitmask actorMask = m_masks[id];
        if (requiredMask & actorMask == requiredMask) {
            updateSystemReferences(system, id);
            system->process();
        }
    }
}

void h_core::Scene::drawSystem(h_core::System* system) {
    ComponentBitmask requiredMask = system->getMask();

    for (ActorId id = 0; id < SCENE_MAX_ACTORS; id++) {
        ComponentBitmask actorMask = m_masks[id];
        if (requiredMask & actorMask == requiredMask) {
            updateSystemReferences(system, id);
            system->draw();
        }
    }
}