#pragma once

#include <stdint.h>

#include "core/actor.h"
#include "core/componentbitmask.h"
#include "core/actorspec.h"
#include "core/scenespec.h"
#include "core/system.h"
#include "core/transform.h"
#include "core/systems/render/mesh.h"
#include "core/systems/script/script.h"

#define SCENE_MAX_ACTORS 1024

namespace h_core {
class Scene {
  public:
    Scene() = default;

    /// @brief set up a scene based on a scene spec
    /// @param sceneSpec scene spec to use
    void initFromSceneSpecAssetIndex(
        h_core::Assets* assets, h_core::AssetIndex sceneSpecIndex);

    ActorId addActor(h_core::ActorSpec* actorSpec);

    void initSystem(h_core::System* system);

    void processSystem(h_core::System* system);

    void drawSystem(h_core::System* system);

  private:
    void updateSystemReferences(h_core::System* system, h_core::ActorId id);

    ActorId m_nextId = 0;

    h_core::ComponentBitmask m_masks[SCENE_MAX_ACTORS] = {};
    h_core::Transform m_transforms[SCENE_MAX_ACTORS] = {};
    h_core::render::Mesh m_meshes[SCENE_MAX_ACTORS] = {};
    h_core::script::Script m_scripts[SCENE_MAX_ACTORS] = {};
};
}  // namespace h_core