#pragma once

#include <tinystl/allocator.h>
#include <tinystl/vector.h>

#include "core/asset.h"
#include "core/actorspec.h"

namespace h_core {
class SceneSpec : public Asset {
  public:
    void loadFromYaml(YAML::Node yaml);

    tinystl::vector<ActorSpec*> actorSpecs;
};
}  // namespace h_core
