//
// Created by xavier.olmstead on 9/7/2024.
//
#include "core/physics/physics.h"

#define PHYSICS_GRAVITATIONAL_CONSTANT 0.0000000000667f

uint32_t h_core::physics::Physics::init(h_core::RuntimeEngine* engine) {
    return 0;
}

void h_core::physics::Physics::endFrame() {
    RigidbodyAsset* rb = rigidbodyComp->rigidbody;
    float deltaTime = engine->getDeltaSecs();

    rb->acceleration += rb->accumulatedForce * rb->inverseMass * deltaTime;
    rb->clearForces();

    rb->acceleration += calculateGravity() * deltaTime;

    rb->velocity += rb->acceleration * deltaTime;
    transform->position += rb->velocity * deltaTime;
}
void h_core::physics::Physics::draw() {
    for(RigidbodyComp* rbc : allRigidbodies) { //make sure we don't have duplicates in the static list O(n) every frame
        if(rbc == rigidbodyComp) {
            return;
        }
    }

    allRigidbodies.push_back(rigidbodyComp);
}

h_core::math::Vector3 h_core::physics::Physics::calculateGravity() {
    h_core::math::Vector3 gravitationalForce = h_core::math::Vector3();

    for (uint32_t mainRBIndex = 0; mainRBIndex < allRigidbodies.size() - 1; mainRBIndex++) {
        for (uint32_t secondaryRBIndex = mainRBIndex + 1; secondaryRBIndex < allRigidbodies.size();
             secondaryRBIndex++) {
            RigidbodyAsset* secondaryRB = allRigidbodies[secondaryRBIndex]->rigidbody;
            RigidbodyAsset* mainRB = allRigidbodies[mainRBIndex]->rigidbody;
            h_core::math::Vector3 direction = secondaryRB->position - mainRB->position;

            gravitationalForce += direction.normalize() * ((PHYSICS_GRAVITATIONAL_CONSTANT * mainRB->mass * secondaryRB->mass) /
                                                           direction.getSquaredLength());
        }
    }

    return gravitationalForce;
}
