#include <string>

#include "core/engine.h"
#include "editor/editorengine.h"

int main(int, char*[]) {
    h_editor::EditorEngine* engine = new h_editor::EditorEngine();

    h_core::project::Project project {};
    project.loadFromFile("assets/project.yml", "");

    const uint32_t engineInitResult = engine->init(&project);
    if (engineInitResult != 0) { return static_cast<int>(engineInitResult); }

    engine->run();
    engine->destroy();

    return 0;
}