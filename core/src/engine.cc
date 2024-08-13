#include "core/engine.h"

h_core::Engine::Engine() {}

h_core::Engine::~Engine() {
    delete m_window;
}

void h_core::Engine::loadProject(h_core::Project project) {
    m_project = project;

#define ENGINE_WINDOW_TITLE_MAX_LENGTH 1024
    char windowTitle[ENGINE_WINDOW_TITLE_MAX_LENGTH] = "hydrogen runtime - ";
    bx::strCat(windowTitle, ENGINE_WINDOW_TITLE_MAX_LENGTH,
               project.projectName);
    m_window = new h_core::Window(windowTitle, 1600, 900, false);
}

void h_core::Engine::run() {
    bool engineRunning = true;
    while (engineRunning) {
        m_window->postEventsToQueue(&m_events);

        for (uint32_t eventIndex = 0; eventIndex < m_events.getSize();
             eventIndex++) {
            switch (m_events.getHeadPointer()[eventIndex].type) {
                case ENGINE_EVENT_QUIT:
                    engineRunning = false;
                    break;
                default:
                    break;
            }
        }

        m_events.clear();
    }
}
