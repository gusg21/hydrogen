#include "core/actorspec.h"


void h_core::ActorSpec::initFromYaml(h_core::Assets* assets, YAML::Node yaml) {
    mask = yaml["mask"].as<h_core::ComponentBitmask>();
    transform.initFromYaml(yaml["transform"]);
}