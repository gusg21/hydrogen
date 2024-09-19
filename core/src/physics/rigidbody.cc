//
// Created by xavier.olmstead on 9/7/2024.
//
#include "core/physics/rigidbody.h"

void h_core::physics::Rigidbody::initFromYaml(YAML::Node yaml) {
    //TODO: read data from yaml file

    mass = yaml["rigidbody"]["mass"].as<uint32_t>(1);
    inverseMass = 1.f / mass;
}

void h_core::physics::Rigidbody::clearForces() {
    accumulatedForce = h_core::math::Vector3(0);
}