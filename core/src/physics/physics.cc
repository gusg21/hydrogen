//
// Created by xavier.olmstead on 9/7/2024.
//
#include "core/physics/physics.h"

#define PHYSICS_GRAVITATIONAL_CONSTANT 0.0000000000667f

uint32_t h_core::physics::Physics::init(h_core::RuntimeEngine* engine) {
    h_core::RuntimeSystem::init(engine);
    return 0;
}

void h_core::physics::Physics::endFrame() {
    float deltaTime = engine->getDeltaSecs();

    for(RigidbodyAsset* rb : allRigidbodies) {
        rb->position = transform->position;
        rb->acceleration += rb->accumulatedForce * rb->inverseMass * deltaTime;
        rb->clearForces();

        rb->acceleration += calculateGravity() * deltaTime;

        rb->velocity += rb->acceleration * deltaTime;
        transform->position += rb->velocity * deltaTime;
    }
}
void h_core::physics::Physics::draw() {
    RigidbodyAsset* rb = engine->getRuntimeAssets()->getAssetByIndex<h_core::physics::RigidbodyAsset>(rigidbodyComp->rigidbody);
    if(rb == nullptr) return;

    for(RigidbodyAsset* rbc : h_core::physics::Physics::allRigidbodies) { //make sure we don't have duplicates in the static list O(n) every frame
        if(rbc == rb) {
            return;
        }
    }



    HYLOG_INFO("PHYSICS: Added rigidbody on index: %zu actor", rigidbodyComp->rigidbody);
    h_core::physics::Physics::allRigidbodies.push_back(rb);
}

h_core::math::Vector3 h_core::physics::Physics::calculateGravity() {
    h_core::math::Vector3 gravitationalForce = h_core::math::Vector3();

    for (uint32_t mainRBIndex = 0; mainRBIndex < allRigidbodies.size() - 1; mainRBIndex++) {
        for (uint32_t secondaryRBIndex = mainRBIndex + 1; secondaryRBIndex < allRigidbodies.size();
             secondaryRBIndex++) {
            RigidbodyAsset* secondaryRB = allRigidbodies[secondaryRBIndex];
            RigidbodyAsset* mainRB = allRigidbodies[mainRBIndex];
            h_core::math::Vector3 direction = secondaryRB->position - mainRB->position;

            gravitationalForce += direction.normalize() * ((PHYSICS_GRAVITATIONAL_CONSTANT * mainRB->mass * secondaryRB->mass) /
                                                           direction.getSquaredLength());
        }
    }

    return gravitationalForce;
}
