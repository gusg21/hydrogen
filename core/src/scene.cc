#pragma once

#include <stdio.h>

#include "core/actorspec.h"
#include "core/scene.h"
#include "core/system.h"

void h_core::Scene::initFromSceneSpec(
    const h_core::SceneSpec* const sceneSpec) {
    for (uint32_t actorSpecIndex = 0; actorSpecIndex < sceneSpec->actorSpecs.size(); actorSpecIndex++) {
        addActor(sceneSpec->actorSpecs[actorSpecIndex]);
    }
}

h_core::ActorId h_core::Scene::addActor(ActorSpec spec) {
    ActorId newId = m_nextId;
    m_masks[newId] = spec.mask;
    m_transforms[newId] = spec.transform;
    printf("adding actor id %d, mask %d\n", newId, m_masks[newId]);
    m_nextId++;
    return newId;
}

void h_core::Scene::runSystem(h_core::System* system) {
    for (ActorId id = 0; id < SCENE_MAX_ACTORS; id++) {
        ComponentBitmask requiredMask = system->getMask();
        ComponentBitmask actorMask = m_masks[id];
        //printf("req %d, actor %d\n", requiredMask, actorMask);
        if (requiredMask & actorMask == requiredMask) {
            system->transform = &m_transforms[id];
            system->process();
        }
    }
}