#include "core/serverengine.h"

h_core::ServerEngine* engine;
h_core::project::Project project;

extern "C" {

void create_engine() {
   engine = new h_core::ServerEngine();
}

void load_project(char* project_path) {
    project.loadFromFile(project_path);
    engine->init(&project);
}

}