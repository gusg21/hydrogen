//
// Created by xavier.olmstead on 9/7/2024.
//
#include "core/physics/physics.h"

#define PHYSICS_GRAVITATIONAL_CONSTANT 0.00000000006674f

uint32_t h_core::physics::Physics::init(h_core::RuntimeEngine* engine) {
    h_core::RuntimeSystem::init(engine);
    return 0;
}

void h_core::physics::Physics::endFrame() {
    float deltaTime = engine->getDeltaSecs();

    for(Rigidbody* rb : allRigidbodies) {
        rb->acceleration += rb->accumulatedForce * rb->inverseMass * deltaTime;
        rb->clearForces();

        rb->acceleration += calculateGravity(rb) * deltaTime;

        rb->velocity += rb->acceleration * deltaTime;
        rb->position += rb->velocity * deltaTime;
    }
}
void h_core::physics::Physics::draw() {
    if(rigidbody == nullptr) return;
    transform->position = rigidbody->position;

    for(Rigidbody* rbc : h_core::physics::Physics::allRigidbodies) { //make sure we don't have duplicates in the static list O(n) every frame
        if(rbc == rigidbody) {
            return;
        }
    }
    //HYLOG_INFO("PHYSICS: Added rigidbody on index: %zu actor", rigidbodyComp);
    h_core::physics::Physics::allRigidbodies.push_back(rigidbody);
}

h_core::math::Vector3 h_core::physics::Physics::calculateGravity(Rigidbody* rb) {
    h_core::math::Vector3 gravitationalForce = h_core::math::Vector3();

    for (auto & otherRB : allRigidbodies) {
        if(otherRB == rb) { continue; }

        h_core::math::Vector3 direction = otherRB->position - rb->position;

        if(direction.getSquaredLength() < 0.001f) continue;

        gravitationalForce += direction.normalize() * ((PHYSICS_GRAVITATIONAL_CONSTANT * rb->mass * otherRB->mass) /
                                                       direction.getSquaredLength());
    }

    return gravitationalForce;
}
