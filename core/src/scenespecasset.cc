#include "core/scenespecasset.h"

#include <string>
#include <vector>

#include "yaml-cpp/yaml.h"

#include "core/assets.h"

uint32_t h_core::SceneSpecAsset::initFromYaml(
    h_core::Assets* assets, h_core::Systems* systems, YAML::Node yaml) {
    std::vector<YAML::Node> actorSpecYamls =
        yaml["actors"].as<std::vector<YAML::Node>>(std::vector<YAML::Node> {});
    actorSpecIndices.clear();

    for (YAML::Node actorSpecYaml : actorSpecYamls) {
        h_core::AssetIndex actorSpecIndex =
            actorSpecYaml["index"].as<h_core::AssetIndex>(UINT32_MAX);

        if (actorSpecIndex == UINT32_MAX) {
            return SCENESPEC_INIT_FAIL_MISSING_ACTOR_SPEC_PATH;
        }

        actorSpecIndices.push_back(actorSpecIndex);
    }

    return 0;
}
uint32_t h_core::SceneSpecAsset::initFromFile(
    h_core::Assets* assets, h_core::Systems* systems, std::string filepath) {
    printf("ERROR: SCENESPEC: trying to load scene from file");

    return 1;
}