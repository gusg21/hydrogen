#pragma once

#include "core/engineevents.h"
#include "core/eventqueue.h"
#include "core/math/math.h"
#include "core/project.h"
#include "core/scene.h"
#include "core/system.h"
#include "core/systems.h"
#include "core/window.h"

#define ENGINE_SYSTEM_COUNT 3

#define ENGINE_INIT_FAIL_BAD_WINDOW_INIT 1
#define ENGINE_INIT_FAIL_BAD_SYSTEM_INIT 2

namespace h_core {
class Engine {
  public:
    Engine() = default;
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    ~Engine() = default;

    /// @brief set up the engine with a given project
    /// @param project the project
    uint32_t init(h_core::Assets* assets, h_core::Project* project);

    /// @brief clean up the engine
    void destroy();

    /// @brief run the game loop (blocking)
    void run();

    [[nodiscard]] uint32_t getWidth() const;
    [[nodiscard]] uint32_t getHeight() const;
    [[nodiscard]] h_core::math::Color getClearColor() const;

  private:
    h_core::Systems m_systems {};

    h_core::Assets* m_assets = nullptr;
    h_core::Project* m_project = nullptr;
    h_core::Window* m_window = nullptr;
    h_core::EventQueue m_events {};
    h_core::Scene m_scene {};

    uint32_t m_windowWidth = 1600;
    uint32_t m_windowHeight = 900;
    h_core::math::Color m_clearColor;
};
}  // namespace h_core
