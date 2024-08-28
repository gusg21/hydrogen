//
// Created by angus.goucher on 8/28/2024.
//

#include "core/systems.h"

uint32_t h_core::Systems::init(h_core::Engine* engine) {
    renderer->init(engine);
    gravity->init(engine);
    scripting->init(engine);
}

void h_core::Systems::destroy() {
    renderer->destroy();
    delete renderer;
    renderer = nullptr;

    gravity->destroy();
    delete gravity;
    gravity = nullptr;

    scripting->destroy();
    delete scripting;
    scripting = nullptr;
}


void h_core::Systems::updateSystemReferences(
    h_core::System* system, h_core::Scene* scene, h_core::ActorId id) {
    system->actorId = id;
    system->transform = &scene->transforms[id];
    system->mesh = &scene->meshes[id];
    system->script = &scene->scripts[id];
}

void h_core::Systems::initScene(h_core::Scene* scene) {
    for (uint32_t systemIndex = 0; systemIndex < SYSTEMS_SYSTEM_COUNT;
         systemIndex++) {
        // EVIL! EVIL! EVIL!
        System* system = &((System*)this)[systemIndex];
        ComponentBitmask requiredMask = system->getMask();

        for (ActorId id = 0; id < SCENE_MAX_ACTORS; id++) {
            ComponentBitmask actorMask = scene->masks[id];
            if ((requiredMask & actorMask) == requiredMask) {
                updateSystemReferences(system, scene, id);
                system->initPerActor();
            }
        }
    }
}

void h_core::Systems::beginFrame() {
    for (uint32_t systemIndex = 0; systemIndex < SYSTEMS_SYSTEM_COUNT;
         systemIndex++) {
        // EVIL! EVIL! EVIL!
        System* system = &((System*)this)[systemIndex];
        system->beginFrame();
    }
}

void h_core::Systems::processScene(h_core::Scene* scene) {
    for (uint32_t systemIndex = 0; systemIndex < SYSTEMS_SYSTEM_COUNT;
         systemIndex++) {
        // EVIL! EVIL! EVIL!
        System* system = &((System*)this)[systemIndex];
        ComponentBitmask requiredMask = system->getMask();

        for (ActorId id = 0; id < SCENE_MAX_ACTORS; id++) {
            ComponentBitmask actorMask = scene->masks[id];
            if ((requiredMask & actorMask) == requiredMask) {
                updateSystemReferences(system, scene, id);
                system->process();
            }
        }
    }
}

void h_core::Systems::drawScene(h_core::Scene* scene) {
    for (uint32_t systemIndex = 0; systemIndex < SYSTEMS_SYSTEM_COUNT;
         systemIndex++) {
        // EVIL! EVIL! EVIL!
        System* system = &((System*)this)[systemIndex];
        ComponentBitmask requiredMask = system->getMask();

        for (ActorId id = 0; id < SCENE_MAX_ACTORS; id++) {
            ComponentBitmask actorMask = scene->masks[id];
            if ((requiredMask & actorMask) == requiredMask) {
                updateSystemReferences(system, scene, id);
                system->draw();
            }
        }
    }
}

void h_core::Systems::endFrame() {
    for (uint32_t systemIndex = 0; systemIndex < SYSTEMS_SYSTEM_COUNT;
         systemIndex++) {
        // EVIL! EVIL! EVIL!
        System* system = &((System*)this)[systemIndex];
        system->endFrame();
    }
}

