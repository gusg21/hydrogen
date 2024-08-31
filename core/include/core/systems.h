//
// Created by angus.goucher on 8/28/2024.
//

#pragma once

#include "core/actor.h"

#define SYSTEMS_SYSTEM_COUNT         3
#define SYSTEMS_INIT_FAIL_BAD_SYSTEM 1

namespace h_core {
  class Engine;
  class Scene;
  class System;
  namespace systems {
    class Gravity;
  }
  namespace render {
    class Renderer;
  }
  namespace script {
    class Scripting;
  }

class Systems {
  public:
    uint32_t init(h_core::Engine* engine);
    void destroy();

    void initScene(h_core::Scene* scene);
    void beginFrame();
    void processScene(h_core::Scene* scene);
    void drawScene(h_core::Scene* scene);
    void endFrame();

    h_core::systems::Gravity* gravity = nullptr;
    h_core::render::Renderer* renderer = nullptr;
    h_core::script::Scripting* scripting = nullptr;
    // Make sure to update SYSTEMS_SYSTEM_COUNT or the whole thing will crash!

  private:
    void updateSystemReferences(
        h_core::System* system, h_core::Scene* scene, h_core::ActorId id);
};

}  // namespace h_core