#include <stdio.h>

#include <core/engine.h>

void pmat(h_core::math::Mat4x4 m) {
    for (uint32_t i = 0; i < 4; i++) {
        printf(
            "%f %f %f %f\n", m.matrix[0 + i], m.matrix[4 + i], m.matrix[8 + i],
            m.matrix[12 + i]);
    }
}

int main(int argc, char* args[]) {
    // printf("Runtime compiled successfully!\n");

    h_core::math::Mat4x4 a {};
    h_core::math::Mat4x4 b {};

    // clang-format off
    float adata[16] = {
        1, 0, 2, 1,
        2, 1, 3, 1,
        1, 0, 4, 1,
        1, 1, 1, 1
    };
    float bdata[16] = {
        2, 6, 1, 1,
        5, 7, 8, 1,
        1, 1, 1, 1,
        1, 1, 1, 1
    };


    for (uint32_t i = 0; i < 16; i++) {
        a.matrix[i] = adata[i];
    }
    for (uint32_t i = 0; i < 16; i++) {
        b.matrix[i] = bdata[i];
    }

    h_core::math::Mat4x4 out = h_core::math::Mat4x4::multiply(a, b);
    pmat(out);

    h_core::Engine* engine = new h_core::Engine();

    h_core::Project project {};
    project.projectName = "Hello World!";

    uint32_t engineInitResult = engine->init(&project);
    if (engineInitResult != 0) { return engineInitResult; }

    project.initialSceneSpec =
    project.assets.getOrLoadAsset<h_core::SceneSpec>(
        "assets/test_scene.yml");

    engine->run();
    engine->destroy();

    return 0;
}