#include <stdio.h>

#include <core/runtimeengine.h>

int main(int argc, char* args[]) {
    h_core::RuntimeEngine* engine = new h_core::RuntimeEngine();

    h_core::project::Project project {};
    project.loadFromFile("assets/project.yml", "");

    uint32_t engineInitResult = engine->init(&project);
    if (engineInitResult != 0) { return engineInitResult; }

    engine->run();
    engine->destroy();

    return 0;
}