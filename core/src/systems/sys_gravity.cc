#include "core/systems/sys_gravity.h"

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
    transform->rotation =
        h_core::math::Quaternion::rotate(transform->rotation, 0.001f, h_core::math::Vector3 {0.f, 1.f, 0.f});
}

h_core::ComponentBitmask h_core::systems::Gravity::getMask() {
    return (1 << 0);
}