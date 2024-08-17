#include "core/actorspec.h"


uint32_t h_core::ActorSpec::initFromYaml(h_core::Assets* assets, YAML::Node yaml) {
    // TODO: Error handling

    mask = yaml["mask"].as<h_core::ComponentBitmask>();
    transform.initFromYaml(yaml["transform"]);
    
    return 0;
}