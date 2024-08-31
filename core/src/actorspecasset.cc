#include "core/actorspecasset.h"

#include "core/systems/render/meshasset.h"
#include "core/systems/script/scriptasset.h"

uint32_t h_core::ActorSpecAsset::initFromYaml(
    h_core::Assets* assets, h_core::Systems* systems, YAML::Node yaml) {
    // TODO: Error handling

    if (!yaml["mask"].IsDefined())
        printf("WARN: ACTORSPEC: No mask on actor spec!\n");

    mask = yaml["mask"].as<h_core::ComponentBitmask>(0);

    if (yaml["transform"].IsDefined())
        transform.initFromYaml(yaml["transform"]);
    else
        printf("WARN: ACTORSPEC: No transform on actor spec!\n");

    // TODO: Convert to model loading
    if (yaml["model"].IsDefined()) {
        meshIndex = assets->getOrLoadAsset<h_core::render::MeshAsset>(
            yaml["mesh"]["file"].as<std::string>(""));
    }

    if (yaml["script"].IsDefined())
        scriptIndex = assets->getOrLoadAsset<h_core::script::ScriptAsset>(
            yaml["script"]["file"].as<std::string>(""));

    return 0;
}