#pragma once

#include <stdint.h>

#include "core/actor.h"
#include "core/actorspec.h"
#include "core/scenespec.h"
#include "core/transform.h"
#include "core/system.h"

#define SCENE_MAX_ACTORS 1024

namespace h_core {
typedef uint32_t ComponentBitmask;
typedef uint32_t ActorId;

class Scene {
  public:
    Scene() = default;

    /// @brief set up a scene based on a scene spec
    /// @param sceneSpec scene spec to use
    void initFromSceneSpecAssetIndex(
        h_core::Assets* assets, h_core::AssetIndex sceneSpecIndex);

    ActorId addActor(h_core::ActorSpec* actorSpec);

    void runSystem(h_core::System* system);

  private:
    ActorId m_nextId = 0;

    h_core::ComponentBitmask m_masks[SCENE_MAX_ACTORS] = {};
    h_core::Transform m_transforms[SCENE_MAX_ACTORS] = {};
};
}  // namespace h_core