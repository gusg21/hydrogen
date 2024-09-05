#include "core/serverengine.h"

#if __WIN32__
#define WRAP_DECL __declspec(dllexport)
#else
#define WRAP_DECL
#endif

h_core::ServerEngine* engine;
h_core::project::Project project {};

extern "C" {

WRAP_DECL void create_engine() {
    SDL_Log("INFO: WRAP: Creating engine\n");
    engine = new h_core::ServerEngine();
}

WRAP_DECL void load_project(const char* project_path) {
    SDL_Log("INFO: WRAP: Loading project name @ %p\n", project_path);
    project.loadFromFile(project_path);
    engine->init(&project);
}

}