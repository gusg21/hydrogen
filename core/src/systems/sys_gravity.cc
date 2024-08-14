#include "core/systems/sys_gravity.h"

#include <stdio.h>

#include <core/scene.h>

void h_core::systems::Gravity::process() {
    transform->position.y += 0.5f;
    //printf("y pos %f\n", transform->position.y);
}

h_core::ComponentBitmask h_core::systems::Gravity::getMask() {
    return (1 << 0);
}