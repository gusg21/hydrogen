#include <stdio.h>

#include <core/engine.h>

int main() {
    printf("Runtime compiled successfully!\n");

    h_core::Engine* engine = new h_core::Engine();

    h_core::Project project {};
    project.projectName = "Hello World!";
    project.initialSceneSpec = project.assets.getOrLoadAsset<h_core::SceneSpec>("assets/test_scene.yml");

    engine->init(project);
    engine->run();
    engine->destroy();
}