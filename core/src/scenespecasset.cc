#include "core/scenespecasset.h"

#include <string>
#include <vector>

#include "yaml-cpp/yaml.h"

#include "core/assets.h"

uint32_t h_core::SceneSpecAsset::initFromYaml(const YAML::Node& yaml) {
    Asset::initFromYaml(yaml);

    std::vector<YAML::Node> actorSpecYamls = yaml["actors"].as<std::vector<YAML::Node>>(std::vector<YAML::Node> {});
    actorSpecIndices.clear();

    for (YAML::Node actorSpecYaml : actorSpecYamls) {
        h_core::AssetIndex actorSpecIndex = actorSpecYaml["index"].as<h_core::AssetIndex>(UINT32_MAX);

        if (actorSpecIndex == UINT32_MAX) { return SCENESPEC_INIT_FAIL_MISSING_ACTOR_SPEC_PATH; }

        actorSpecIndices.push_back(actorSpecIndex);

        //okay, so here's the deal. I'm not sure why having a lot of actors is bad, they are already referencing the expensive stuff
        //like scripts and models, otherwise they need transform data, which should be unique to every actor, I really think just
        //saving a lot of actors to our scene yaml makes more sense (especially if we keep transform data in our actor yaml)

        if(actorSpecYaml["overrides"].IsDefined()) {
            YAML::Node transformYaml = actorSpecYaml["overrides"]["transform"];
            //TODO: apply transformYAML to actor for updated info, not sure where to apply rn

            int test = transformYaml["position"]["x"].as<int>(0);
        }
    }

    return 0;
}
