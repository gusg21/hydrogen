#include "core/actorspecasset.h"

#include "core/log.h"
#include "core/systems/render/meshasset.h"
#include "core/systems/script/scriptasset.h"

uint32_t h_core::ActorSpecAsset::initFromYaml(
    h_core::Assets* assets, YAML::Node yaml) {
    // TODO: Error handling

    if (!yaml["mask"].IsDefined())
        HYLOG_WARN("ACTORSPEC: No mask on actor spec!\n");

    mask = yaml["mask"].as<h_core::ComponentBitmask>(0);

    if (yaml["transform"].IsDefined())
        transform.initFromYaml(yaml["transform"]);
    else
        HYLOG_WARN("ACTORSPEC: No transform on actor spec!\n");

    // TODO: Convert to model loading
    if (yaml["model"].IsDefined()) {
        meshIndex = yaml["model"]["index"].as<h_core::AssetIndex>(ASSET_INDEX_BAD);
    }

    if (yaml["script"].IsDefined())
        scriptIndex = yaml["script"]["index"].as<h_core::AssetIndex>(ASSET_INDEX_BAD);

    if(yaml["rigidbody"].IsDefined())
        rigidbody = new physics::Rigidbody(transform.position, 1000000);

    return 0;
}