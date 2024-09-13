#include "core/scenespecasset.h"

#include <string>
#include <vector>

#include "yaml-cpp/yaml.h"

#include "core/assets.h"

uint32_t h_core::SceneSpecAsset::initFromYaml(
    h_core::Assets* assets, const h_core::AssetDescription& desc, const YAML::Node& yaml) {
    Asset::initFromYaml(assets, desc, yaml);

    std::vector<YAML::Node> actorSpecYamls = yaml["actors"].as<std::vector<YAML::Node>>(std::vector<YAML::Node> {});
    actorSpecIndices.clear();

    for (YAML::Node actorSpecYaml : actorSpecYamls) {
        h_core::AssetIndex actorSpecIndex = actorSpecYaml["index"].as<h_core::AssetIndex>(UINT32_MAX);

        if (actorSpecIndex == UINT32_MAX) { return SCENESPEC_INIT_FAIL_MISSING_ACTOR_SPEC_PATH; }

        actorSpecIndices.push_back(actorSpecIndex);
    }

    return 0;
}
