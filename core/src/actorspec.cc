#include "core/actorspec.h"


uint32_t h_core::ActorSpec::initFromYaml(
    h_core::Assets* assets, YAML::Node yaml) {
    // TODO: Error handling

    if (!yaml["mask"].IsDefined())
        printf("WARN: ACTORSPEC: No mask on actor spec!\n");

    mask = yaml["mask"].as<h_core::ComponentBitmask>(0);

    if (yaml["transform"].IsDefined())
        transform.initFromYaml(yaml["transform"]);
    else
        printf("WARN: ACTORSPEC: No transform on actor spec!\n");

    // TODO: Convert to model loading
    if (yaml["model"].IsDefined()) mesh.initFromYaml(assets, yaml["model"]);

    if (yaml["script"].IsDefined())
        assets->getOrLoadAsset<h_core::script::ScriptAsset>(
            yaml["script"]["file"].as<std::string>(""));

    return 0;
}