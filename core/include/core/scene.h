#pragma once

#include "core/actorid.h"
#include "core/actorspecasset.h"
#include "core/componentbitmask.h"
#include "core/physics/rigidbody.h"
#include "core/scenespecasset.h"
#include "core/render/modelcomp.h"
#include "core/script/scriptcomp.h"
#include "core/transform.h"

#define SCENE_MAX_ACTORS 4096

namespace h_core {
class Scene {
  public:
    Scene() = default;

    void doGUI(h_core::Assets* assets);

    /// @brief set up a scene based on a scene spec
    /// @param sceneSpec scene spec to use
    void addActorsFromSceneSpec(
        h_core::Assets* assets, h_core::AssetIndex sceneSpecIndex,
        asIScriptContext* scriptingContext);

    ActorId addActor(
        h_core::Assets* assets, h_core::AssetIndex actorSpecIndex, asIScriptContext* scriptingContext);

    h_core::ComponentBitmask masks[SCENE_MAX_ACTORS] = {};
    h_core::Transform transforms[SCENE_MAX_ACTORS] = {};
    h_core::render::ModelComp meshes[SCENE_MAX_ACTORS] = {};
    h_core::script::ScriptComp scripts[SCENE_MAX_ACTORS] = {};
    h_core::physics::Rigidbody* rigidbodies[SCENE_MAX_ACTORS] = {};

    std::unordered_map<uint32_t, h_core::Component*[SCENE_MAX_ACTORS]> blah {};

  private:
    ActorId m_nextId = 0;
    size_t m_numActors = 0;
};
}  // namespace h_core