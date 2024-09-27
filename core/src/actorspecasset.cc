#include "core/actorspecasset.h"

#include "core/log.h"
#include "core/render/modelasset.h"
#include "core/script/scriptasset.h"

uint32_t h_core::ActorSpecAsset::initFromYaml(const std::string& basePath, const YAML::Node& yaml) {
    h_core::Asset::initFromYaml(basePath, yaml);

    // TODO: Error handling

    if (!yaml["mask"].IsDefined()) HYLOG_WARN("ACTORSPEC: No mask on actor spec!\n");

    mask = yaml["mask"].as<h_core::ComponentBitmask>(0);

    if (yaml["transform"].IsDefined())
        transform.initFromYaml(yaml["transform"]);
    else
        HYLOG_WARN("ACTORSPEC: No transform on actor spec!\n");

    // TODO: Convert to model loading
    if (yaml["model"].IsDefined()) { meshIndex = yaml["model"]["index"].as<h_core::AssetIndex>(ASSET_INDEX_BAD); }

    if (yaml["script"].IsDefined()) scriptIndex = yaml["script"]["index"].as<h_core::AssetIndex>(ASSET_INDEX_BAD);

    if (yaml["rigidbody"].IsDefined()) {
        rigidbody = new physics::Rigidbody();
        rigidbody->initFromYaml(yaml);
    }
    return 0;
}
void h_core::ActorSpecAsset::saveToYaml(YAML::Node& yaml) {
    // Create YAML
    yaml["mask"] = mask;
    yaml["transform"] = transform.saveToYaml();

    if(meshIndex != ASSET_INDEX_BAD) {
        yaml["model"]["index"] = meshIndex;
    }

    if(scriptIndex != ASSET_INDEX_BAD) {
        yaml["script"]["index"] = scriptIndex;
    }
}