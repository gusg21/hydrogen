#include <stdio.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

    // h_core::math::Mat4x4 a {};
    // h_core::math::Mat4x4 b {};

    // // clang-format off
    // float adata[16] = {
    //     1, 0, 2, 1,
    //     2, 1, 3, 1,
    //     1, 0, 4, 1,
    //     1, 1, 1, 1
    // };
    // float bdata[16] = {
    //     2, 6, 1, 1,
    //     5, 7, 8, 1,
    //     1, 1, 1, 1,
    //     1, 1, 1, 1
    // };


    // for (uint32_t i = 0; i < 16; i++) {
    //     a.matrix[i] = adata[i];
    // }
    // for (uint32_t i = 0; i < 16; i++) {
    //     b.matrix[i] = bdata[i];
    // }

    // h_core::math::Mat4x4 out = h_core::math::Mat4x4::multiply(a, b);
    // pmat(out);

    h_core::Transform trans {};
    trans.position = h_core::math::Vector3 { 1.f, 1.f, 1.f };
    trans.rotation = h_core::math::Quaternion { 0.f, 0.5f, 0.f, 1.f };
    trans.scale = h_core::math::Vector3 { 2.f, 2.f, 2.f };
    pmat(trans.getMatrix());

    glm::mat4 glmTrans = glm::mat4(1.0f);
    glmTrans = glm::translate(glmTrans, { 1.f, 1.f, 1.f });
    glmTrans *= glm::mat4_cast(glm::quat { 1.0f, 0.f, 0.5f, 0.f });
    glmTrans = glm::scale(glmTrans, { 2.f, 2.f, 2.f });
    printf("%s\n", glm::to_string(glmTrans).c_str());


    h_core::Engine* engine = new h_core::Engine();

    h_core::Project project {};
    project.projectName = "Hello World!";

    uint32_t engineInitResult = engine->init(&project);
    if (engineInitResult != 0) { return engineInitResult; }

    project.initialSceneSpec = project.assets.getOrLoadAsset<h_core::SceneSpec>(
        "assets/test_scene.yml");

    engine->run();
    engine->destroy();

    return 0;
}