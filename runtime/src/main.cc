#include <stdio.h>

#include <core/engine.h>

int main() {
    printf("Runtime compiled successfully!\n");

    h_core::Engine* engine = new h_core::Engine();

    h_core::SceneSpec scene {};
    h_core::ActorSpec actor {};
    actor.mask = 0b1;
    actor.transform.position.y = 10;
    scene.actorSpecs.push_back(actor);

    h_core::ActorSpec actor2 {};
    actor2.mask = 0b1;
    actor2.transform.position.y = 100;
    scene.actorSpecs.push_back(actor2);


    h_core::Project project {};
    project.projectName = "Hello World!";
    project.initialSceneSpec = &scene;

    engine->init(project);
    engine->run();
    engine->destroy();
}