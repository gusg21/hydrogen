#include "core/scenespec.h"

#include <string>
#include <vector>

#include "yaml-cpp/yaml.h"

#include "core/assets.h"

void h_core::SceneSpec::initFromYaml(h_core::Assets* assets, YAML::Node yaml) {
    std::vector<YAML::Node> actorSpecYamls =
        yaml["actors"].as<std::vector<YAML::Node>>();
    actorSpecs = std::vector<h_core::AssetIndex> {};
    actorSpecs.reserve(actorSpecYamls.size());

    for (YAML::Node actorSpecYaml : actorSpecYamls) {
        std::string actorSpecPath = actorSpecYaml["file"].as<std::string>();
        h_core::AssetIndex actorSpecIndex =
            assets->getOrLoadAsset<h_core::ActorSpec>(actorSpecPath);
        actorSpecs.push_back(actorSpecIndex);
    }
}