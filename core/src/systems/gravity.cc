#include "core/systems/gravity.h"

#include "stdio.h"

#include "core/scene.h"

// NOTE: This system is a dummy! Useful for reference and testing but probably
// not in any other cases.

uint32_t h_core::systems::Gravity::init(h_core::RuntimeEngine* engine) {
    h_core::RuntimeSystem::init(engine);

    return 0;
}

void h_core::systems::Gravity::process() {

}