//
// Created by xavier.olmstead on 9/7/2024.
//
#include "core/physics/rigidbodyasset.h"

uint32_t h_core::physics::RigidbodyAsset::initFromYaml(h_core::Assets* assets, YAML::Node node) {
    velocity = h_core::math::Vector3(0);
    acceleration = h_core::math::Vector3(0);
    accumulatedForce = h_core::math::Vector3(0);
    position = h_core::math::Vector3(0);
    //TODO: set position to equal transform position

    mass = 100000; //TODO: read data from yml file
    inverseMass = 1 / mass;

    return 0;
}
void h_core::physics::RigidbodyAsset::clearForces() {
    accumulatedForce = h_core::math::Vector3(0);
}
