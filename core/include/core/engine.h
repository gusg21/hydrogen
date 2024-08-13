#pragma once

#include <bgfx/bgfx.h>

#include "core/project.h"
#include "core/window.h"
#include "core/eventqueue.h"
#include "core/engineevents.h"

namespace h_core {
class Engine {
public:
    Engine();
    Engine(const Engine&)            = delete;
    Engine& operator=(const Engine&) = delete;
    ~Engine();

    void init(h_core::Project project);
    void destroy();
    void run();

private:
    h_core::Project m_project {};
    h_core::Window* m_window = nullptr;
    h_core::EventQueue m_events {};

    // TODO: Temp rendering variables
    bgfx::ViewId m_clearView = 0;
};
}  // namespace h_core
