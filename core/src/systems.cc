//
// Created by angus.goucher on 8/28/2024.
//

#include "core/systems.h"
#include "core/system.h"
#include "core/systems/gravity.h"
#include "core/systems/render/renderer.h"
#include "core/systems/script/scripting.h"

uint32_t h_core::Systems::init(h_core::Engine* engine) {
    uint32_t result;

    for (uint32_t systemIndex = 0; systemIndex < SYSTEMS_SYSTEM_COUNT;
         systemIndex++) {
        // EVIL! EVIL! EVIL!
        System* system = ((System**)this)[systemIndex];
        system->init(engine);
    }

    return 0;
}

void h_core::Systems::destroy() {
    //    renderer->destroy();
    //    delete renderer;
    //    renderer = nullptr;
    //
    //    gravity->destroy();
    //    delete gravity;
    //    gravity = nullptr;
    //
    //    scripting->destroy();
    //    delete scripting;
    //    scripting = nullptr;

    for (uint32_t systemIndex = 0; systemIndex < SYSTEMS_SYSTEM_COUNT;
         systemIndex++) {
        // EVIL! EVIL! EVIL!
        // Note the double-pointer screwery so that we can properly modify the
        // contents of the variable
        System** system = &((System**)this)[systemIndex];
        (*system)->destroy();
        //        delete system;
        *system = nullptr;
    }
}


void h_core::Systems::updateSystemReferences(
    h_core::System* system, h_core::Scene* scene, h_core::ActorId id) {
    system->actorId = id;
    system->transform = &scene->transforms[id];
    system->meshComp = &scene->meshes[id];
    system->script = &scene->scripts[id];
}

void h_core::Systems::initScene(h_core::Scene* scene) {
    for (uint32_t systemIndex = 0; systemIndex < SYSTEMS_SYSTEM_COUNT;
         systemIndex++) {
        // EVIL! EVIL! EVIL!
        System* system = ((System**)this)[systemIndex];
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
        System* system = ((System**)this)[systemIndex];
        system->beginFrame();
    }
}

void h_core::Systems::processScene(h_core::Scene* scene) {
    for (uint32_t systemIndex = 0; systemIndex < SYSTEMS_SYSTEM_COUNT;
         systemIndex++) {
        // EVIL! EVIL! EVIL!
        System* system = ((System**)this)[systemIndex];
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
        System* system = ((System**)this)[systemIndex];
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
        System* system = ((System**)this)[systemIndex];
        system->endFrame();
    }
}
