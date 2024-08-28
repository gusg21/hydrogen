#include "core/scenespec.h"

#include <string>
#include <vector>

#include "yaml-cpp/yaml.h"

#include "core/assets.h"

uint32_t h_core::SceneSpec::initFromYaml(h_core::Assets* assets, h_core::Systems* systems, YAML::Node yaml) {
    std::vector<YAML::Node> actorSpecYamls =
        yaml["actors"].as<std::vector<YAML::Node>>(std::vector<YAML::Node> {});
    actorSpecIndices.clear();

    for (YAML::Node actorSpecYaml : actorSpecYamls) {
        std::string actorSpecPath = actorSpecYaml["file"].as<std::string>("");

        if (actorSpecPath.empty()) {
            return SCENESPEC_INIT_FAIL_MISSING_ACTOR_SPEC_PATH;
        }

        h_core::AssetIndex actorSpecIndex =
            assets->getOrLoadAsset<h_core::ActorSpec>(actorSpecPath);

        actorSpecIndices.push_back(actorSpecIndex);
    }

    return 0;
}