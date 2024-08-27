#include "core/systems/gravity.h"

#include "stdio.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "core/scene.h"

// NOTE: This system is a dummy! Useful for reference and testing but probably
// not in any other cases.

uint32_t h_core::systems::Gravity::init() {
    return 0;
}

void h_core::systems::Gravity::process() {

}

h_core::ComponentBitmask h_core::systems::Gravity::getMask() {
    return TRANSFORM_COMPONENT_BITMASK;
}