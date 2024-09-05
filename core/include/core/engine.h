#pragma once

#include <chrono>

#include "core/engineevents.h"
#include "core/eventqueue.h"
#include "core/input/input.h"
#include "core/math/math.h"
#include "core/project/project.h"
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
    uint32_t init(h_core::project::Project* project);

    /// @brief clean up the engine
    virtual void destroy();

    /// @brief run the game loop (blocking)
    void run();

    [[nodiscard]] uint32_t getWidth() const;
    [[nodiscard]] uint32_t getHeight() const;
    [[nodiscard]] h_core::math::Color getClearColor() const;
    [[nodiscard]] h_core::Scene* getScene();
    [[nodiscard]] h_core::Window* getWindow();
    [[nodiscard]] h_core::Assets* getAssets();
    [[nodiscard]] const h_core::project::Project* getProject();
    [[nodiscard]] h_core::input::Input* getInput();

    [[nodiscard]] double getDeltaSecs() const;
    [[nodiscard]] double getFPS() const;

  protected:
    virtual void doInit() {};
    virtual void doPostLoad() {};
    virtual void prepareScene(h_core::AssetIndex sceneSpecIndex) {};
    virtual void doGUI() {};
    virtual void beginFrame() {};
    virtual void doProcess() {};
    virtual void doDraw() {};
    virtual void endFrame() {};

  private:
    h_core::Assets* m_assets = nullptr;
    h_core::project::Project* m_project = nullptr;
    h_core::Window* m_window = nullptr;
    h_core::input::Input* m_input = nullptr;
    h_core::EventQueue m_events {};
    h_core::Scene m_scene {};

    uint32_t m_windowWidth = 1600;
    uint32_t m_windowHeight = 900;
    h_core::math::Color m_clearColor {};
    uint64_t m_deltaNanosecs = 0;
};
}  // namespace h_core
