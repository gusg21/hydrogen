#pragma once

#include <bgfx/bgfx.h>
#include <bx/allocator.h>

#include "core/engineevents.h"
#include "core/eventqueue.h"
#include "core/project.h"
#include "core/scene.h"
#include "core/window.h"

namespace h_core {
class Engine {
  public:
    Engine() = default;
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    ~Engine() = default;

    /// @brief set up the engine with a given project
    /// @param project the project
    void init(h_core::Project project);

    /// @brief clean up the engine
    void destroy();

    /// @brief run the game loop (blocking)
    void run();

  private:
    h_core::Project m_project {};
    h_core::Window* m_window = nullptr;
    h_core::EventQueue m_events {};
    h_core::Scene m_scene {};

    // TODO: Temp rendering variables
    bgfx::ViewId m_clearView = 0;
};
}  // namespace h_core
