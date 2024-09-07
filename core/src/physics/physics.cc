//
// Created by xavier.olmstead on 9/7/2024.
//
#include "core/physics/physics.h"

uint32_t h_core::physics::Physics::init(h_core::Engine* engine) {
    return 0;
}
void h_core::physics::Physics::endFrame() {

    transform->position.y += rigidbodyComp->rigidbody->getVelocity().y;

}
