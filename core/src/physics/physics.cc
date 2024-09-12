//
// Created by xavier.olmstead on 9/7/2024.
//
#include "core/physics/physics.h"
#include "core/engine.h"
#include "core/physics/rigidbodycomp.h"
#include "core/transform.h"

#define GRAVITATIONAL_CONSTANT 0.0000000000667f

uint32_t h_core::physics::Physics::init(h_core::Engine* engine) {
    return 0;
}

void h_core::physics::Physics::endFrame() {
    RigidbodyAsset* rb = rigidbodyComp->rigidbody;
    float deltaTime = engine->getDeltaSecs();

    rb->acceleration += rb->accumulatedForce * rb->inverseMass;
    rb->clearForces();

    rb->acceleration += calculateGravity();

    rb->velocity += rb->acceleration * deltaTime;
    transform->position += rb->velocity * deltaTime;
}
void h_core::physics::Physics::draw() {
    for(RigidbodyComp* rbc : allRigidbodies) { //make sure we don't have duplicates in the static list
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

            gravitationalForce += direction.normalize() * ((GRAVITATIONAL_CONSTANT * mainRB->mass * secondaryRB->mass) /
                                                           direction.getSquaredLength());
        }
    }

    return gravitationalForce;
}
