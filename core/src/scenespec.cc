#include "core/scenespec.h"

#include <string>
#include <vector>

#include "yaml-cpp/yaml.h"

#include "core/assets.h"

uint32_t h_core::SceneSpec::initFromYaml(h_core::Assets* assets, YAML::Node yaml) {
    // TODO: Error handling

    std::vector<YAML::Node> actorSpecYamls =
        yaml["actors"].as<std::vector<YAML::Node>>();
    actorSpecIndices.clear();

    for (YAML::Node actorSpecYaml : actorSpecYamls) {
        std::string actorSpecPath = actorSpecYaml["file"].as<std::string>();
        h_core::AssetIndex actorSpecIndex =
            assets->getOrLoadAsset<h_core::ActorSpec>(actorSpecPath);

        actorSpecIndices.push_back(actorSpecIndex);
    }

    return 0;
}