//
// Created by angus.goucher on 8/28/2024.
//

#include "core/systems.h"
#include "core/system.h"
#include "core/systems/gravity.h"
#include "core/systems/render/renderer.h"
#include "core/systems/script/scripting.h"

// These macros are evil!
#define FOR_EACH_SYSTEM_PER_ACTOR_IN_SCENE(functionName, ...)                   \
    for (uint32_t systemIndex = 0; systemIndex < SYSTEMS_SYSTEM_COUNT; \
         systemIndex++) {                                              \
        System* system = ((System**)this)[systemIndex];                \
        ComponentBitmask requiredMask = system->getMask();             \
        for (ActorId id = 0; id < SCENE_MAX_ACTORS; id++) {            \
            ComponentBitmask actorMask = scene->masks[id];             \
            if ((requiredMask & actorMask) == requiredMask) {          \
                updateSystemReferences(system, scene, id);             \
                system->functionName(__VA_ARGS__);                     \
            }                                                          \
        }                                                              \
    }

#define FOR_EACH_SYSTEM(functionName, ...)                             \
    for (uint32_t systemIndex = 0; systemIndex < SYSTEMS_SYSTEM_COUNT; \
         systemIndex++) {                                              \
        System* system = ((System**)this)[systemIndex];                \
        system->functionName(__VA_ARGS__);                             \
    }

uint32_t h_core::Systems::init(h_core::Engine* engine) {
    FOR_EACH_SYSTEM(init, engine);

    return 0;
}

void h_core::Systems::destroy() {
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

void h_core::Systems::prepareScene(h_core::Scene* scene) {
    FOR_EACH_SYSTEM_PER_ACTOR_IN_SCENE(initPerActor);
}

void h_core::Systems::doGUI() {
    FOR_EACH_SYSTEM(doGUI);
}

void h_core::Systems::beginFrame() {
    FOR_EACH_SYSTEM(beginFrame);
}

void h_core::Systems::processScene(h_core::Scene* scene) {
    FOR_EACH_SYSTEM_PER_ACTOR_IN_SCENE(process);
}

void h_core::Systems::drawScene(h_core::Scene* scene) {
    FOR_EACH_SYSTEM_PER_ACTOR_IN_SCENE(draw);
}

void h_core::Systems::endFrame() {
    FOR_EACH_SYSTEM(endFrame);
}
