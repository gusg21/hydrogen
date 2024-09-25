//
// Created by Angus Goucher on 9/1/2024.
//

#pragma once

#include <deque>

#include "core/engine.h"
#include "core/runtimeassets.h"
#include "core/runtimesystems.h"
#include "core/runtimeconsole.h"

#define RUNTIMEENGINE_MAX_FPS_SAMPLES 600

namespace h_core {
class RuntimeEngine : public h_core::Engine {
  public:
    void destroy() override;

    h_core::RuntimeAssets* getRuntimeAssets();
    h_core::RuntimeConsole* getConsole();
    h_core::RuntimeSystems* getSystems();

  protected:
    void doInit(const h_core::project::Project* project) override;
    void prepareScene(h_core::AssetIndex sceneSpecIndex) override;
    void beginFrame() override;
    void doProcess() override;
    void doDraw() override;
    void endFrame() override;

    void doGUI();

  private:
    bool m_showImGuiDemo = false;

    std::deque<double> m_fpsSamples { RUNTIMEENGINE_MAX_FPS_SAMPLES, 0.f };
    double m_averageFPS = 0.f;

    h_core::RuntimeAssets* m_assets {};
    h_core::RuntimeSystems m_systems {};
    h_core::RuntimeConsole* m_console {};
};
}  // namespace h_core