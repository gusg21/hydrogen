#pragma once

#include "core/actor.h"
#include "core/scenespec.h"

#define SCENE_MAX_ACTORS 1024

namespace h_core {
class Scene {
  public:
    Scene() = default;

    /// @brief set up a scene based on a scene spec
    /// @param sceneSpec scene spec to use
    void initFromSceneSpec(const h_core::SceneSpec* const sceneSpec);

  private:
    h_core::Actor m_actors[SCENE_MAX_ACTORS] = {};  // Zero-initialized
};
}  // namespace h_core