//
// Created by angus.goucher on 8/28/2024.
//

#pragma once

#include "core/actor.h"
#include "physics/physics.h"

#define SYSTEMS_SYSTEM_COUNT 4

namespace h_core {
class RuntimeEngine;
class Scene;
class RuntimeSystem;
namespace systems {
class Gravity;
}
namespace render {
class Renderer;
}
namespace script {
class Scripting;
}

class RuntimeSystems {
  public:
    uint32_t init(h_core::RuntimeEngine* engine);
    void destroy();

    void prepareScene(h_core::Scene* scene);
    void doGUI();
    void beginFrame();
    void processScene(h_core::Scene* scene);
    void drawScene(h_core::Scene* scene);
    void endFrame();

    h_core::systems::Gravity* gravity = nullptr;
    h_core::render::Renderer* renderer = nullptr;
    h_core::script::Scripting* scripting = nullptr;
    h_core::physics::Physics* physics = nullptr;
    // Make sure to update SYSTEMS_SYSTEM_COUNT or the whole thing will crash!

  private:
    void updateSystemReferences(h_core::RuntimeSystem* system, h_core::Scene* scene, h_core::ActorId id);
};

}  // namespace h_core