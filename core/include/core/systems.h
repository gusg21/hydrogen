//
// Created by angus.goucher on 8/28/2024.
//

#pragma once

#include "core/systems/render/renderer.h"
#include "core/systems/gravity.h"
#include "core/systems/script/scripting.h"
#include "core/scene.h"

#define SYSTEMS_SYSTEM_COUNT 3

namespace h_core {

class Systems {
  public:
    uint32_t init();
    void destroy();

    void initScene(h_core::Scene* scene);
    void beginFrame();
    void processScene(h_core::Scene* scene);
    void drawScene(h_core::Scene* scene);
    void endFrame();

    h_core::systems::Gravity* gravity = nullptr;
    h_core::render::Renderer* renderer = nullptr;
    h_core::script::Scripting* scripting = nullptr;
    // Make sure to update SYSTEMS_SYSTEM_COUNT!

  private:
    void updateSystemReferences(
        h_core::System* system, h_core::Scene* scene, h_core::ActorId id);


};

}