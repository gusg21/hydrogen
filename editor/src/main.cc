#include <string>

#include "core/engine.h"
#include "editor/editor.h"

int main(int argc, char* args[]) {
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    h_editor::Editor* editor = new h_editor::Editor();

    const char* basePath = "";
    if (argc >= 2) {
        basePath = args[1];
    }

    h_core::project::Project project {};
    project.loadFromFile("assets/project.yml", basePath);
    const uint32_t editorInitResult = editor->init(project, basePath);
    if (editorInitResult != 0) { return static_cast<int>(editorInitResult); }

    editor->run();
    editor->destroy();

    return 0;
}