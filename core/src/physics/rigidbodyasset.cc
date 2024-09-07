//
// Created by xavier.olmstead on 9/7/2024.
//
#include "core/physics/rigidbodyasset.h"

uint32_t h_core::physics::RigidbodyAsset::initFromYaml(h_core::Assets* assets, YAML::Node node) {
    velocity = h_core::math::Vector3(0);
    acceleration = h_core::math::Vector3(0);
    accumulatedForce = h_core::math::Vector3(0);

    mass = 100000; //TODO: read data from yml file
    inverseMass = 1 / mass;

    return 0;
}
h_core::math::Vector3 h_core::physics::RigidbodyAsset::getVelocity() {
    return velocity;
}
h_core::math::Vector3 h_core::physics::RigidbodyAsset::getAcceleration() {
    return acceleration;
}
h_core::math::Vector3 h_core::physics::RigidbodyAsset::getAccumulatedForce() {
    return accumulatedForce;
}
float h_core::physics::RigidbodyAsset::getMass() {
    return mass;
}
float h_core::physics::RigidbodyAsset::getInverseMass() {
    return inverseMass;
}
