#include <stdio.h>

#include <core/runtimeengine.h>

int main(int argc, char* args[]) {
    h_core::Engine* engine = new h_core::Engine();

    h_core::project::Project project {};
    project.loadFromFile("assets/project.yml");

    h_core::Assets assets {};

    uint32_t engineInitResult = engine->init(&assets, &project);
    if (engineInitResult != 0) { return engineInitResult; }

    engine->run();
    engine->destroy();

    return 0;
}