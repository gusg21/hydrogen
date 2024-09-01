//
// Created by Angus Goucher on 9/1/2024.
//

#pragma once

#include "core/engine.h"

namespace h_core {
class RuntimeEngine : public h_core::Engine {
  public:
    void destroy() override;

  protected:
    void doInit() override;
    void doPostLoad() override;
    void doGUI() override;
    void prepareScene(h_core::AssetIndex sceneSpecIndex) override;
    void beginFrame() override;
    void doProcess() override;
    void doDraw() override;
    void endFrame() override;

  private:
    bool m_showImGuiDemo = false;

    h_core::Systems m_systems {};
};
}  // namespace h_core