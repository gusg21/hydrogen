#include "core/systems/gravity.h"

#include "stdio.h"

#include "core/scene.h"

// NOTE: This system is a dummy! Useful for reference and testing but probably
// not in any other cases.

uint32_t h_core::systems::Gravity::init(h_core::Engine* engine) {
    h_core::System::init(engine);

    return 0;
}

void h_core::systems::Gravity::process() {
    transform->rotation =
        h_core::math::Quaternion::rotate(transform->rotation, 0.005f, h_core::math::Vector3 {0.f, 1.f, 0.f});
}

h_core::ComponentBitmask h_core::systems::Gravity::getMask() {
    return TRANSFORM_COMPONENT_BITMASK;
}