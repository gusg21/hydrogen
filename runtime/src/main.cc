#include <stdio.h>

#include <core/engine.h>

int main(int argc, char* args[]) {
    // printf("Runtime compiled successfully!\n");

    h_core::Engine* engine = new h_core::Engine();

    h_core::Project project {};
    project.projectName = "Hello World!";

    engine->init(&project);

    project.initialSceneSpec = project.assets.getOrLoadAsset<h_core::SceneSpec>(
        "assets/test_scene.yml");

    engine->run();
    engine->destroy();

    return 0;
}