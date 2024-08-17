#include "core/transform.h"

void h_core::Transform::initFromYaml(YAML::Node yaml) {
    position.x = yaml["position"]["x"].as<float>();
    position.y = yaml["position"]["y"].as<float>();
    position.z = yaml["position"]["z"].as<float>();

    rotation.x = yaml["rotation"]["x"].as<float>();
    rotation.y = yaml["rotation"]["y"].as<float>();
    rotation.z = yaml["rotation"]["z"].as<float>();
    rotation.w = yaml["rotation"]["w"].as<float>();

    scale.x = yaml["scale"]["x"].as<float>();
    scale.y = yaml["scale"]["y"].as<float>();
    scale.z = yaml["scale"]["z"].as<float>();
}

h_core::math::Mat4x4 h_core::Transform::getMatrix() {
    return h_core::math::Mat4x4::createTransformMatrix(
        position, rotation, scale);
}