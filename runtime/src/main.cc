#include <stdio.h>

#include <core/runtimeengine.h>

int main(int argc, char* args[]) {
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    h_core::RuntimeEngine* engine = new h_core::RuntimeEngine();

    h_core::project::Project project {};
    project.loadFromFile("assets/project.hyproject", "");

    uint32_t engineInitResult = engine->init(&project);
    if (engineInitResult != 0) { return engineInitResult; }

    engine->run();
    engine->destroy();

    return 0;
}