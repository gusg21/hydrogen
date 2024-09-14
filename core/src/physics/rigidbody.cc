//
// Created by xavier.olmstead on 9/7/2024.
//
#include "core/physics/rigidbody.h"

void h_core::physics::Rigidbody::initFromYaml(YAML::Node yaml) {
    //TODO: read data from yaml file
    mass = 10000;
    inverseMass = 1.f / mass;
}

void h_core::physics::Rigidbody::clearForces() {
    accumulatedForce = h_core::math::Vector3(0);
}