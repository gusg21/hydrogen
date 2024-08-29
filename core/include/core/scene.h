#pragma once

#include <stdint.h>

#include "core/actor.h"
#include "core/actorspecasset.h"
#include "core/componentbitmask.h"
#include "core/scenespecasset.h"
#include "core/system.h"
#include "core/systems/render/mesh.h"
#include "core/systems/script/scriptcomp.h"
#include "core/transform.h"

#define SCENE_MAX_ACTORS 1024

namespace h_core {
class Scene {
  public:
    Scene() = default;

    /// @brief set up a scene based on a scene spec
    /// @param sceneSpec scene spec to use
    void initFromSceneSpecAssetIndex(
        h_core::Assets* assets, h_core::AssetIndex sceneSpecIndex,
        asIScriptContext* scriptingContext);

    ActorId addActor(
        h_core::ActorSpecAsset* actorSpec, asIScriptContext* scriptingContext);

    h_core::ComponentBitmask masks[SCENE_MAX_ACTORS] = {};
    h_core::Transform transforms[SCENE_MAX_ACTORS] = {};
    h_core::render::Mesh meshes[SCENE_MAX_ACTORS] = {};
    h_core::script::ScriptComp scripts[SCENE_MAX_ACTORS] = {};

  private:
    ActorId m_nextId = 0;
};
}  // namespace h_core