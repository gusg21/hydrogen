#pragma once

#include "core/asset.h"
#include "core/assets.h"
#include "core/componentbitmask.h"
#include "core/transform.h"
#include "core/systems/render/mesh.h"
#include "core/systems/script/scriptcomp.h"

namespace h_core {
class ActorSpec : public Asset {
  public:
    uint32_t initFromYaml(h_core::Assets* assets, YAML::Node yaml);

    h_core::ComponentBitmask mask;
    h_core::Transform transform;
    h_core::render::Mesh mesh;
    h_core::script::ScriptAsset* script;
};
}  // namespace h_core