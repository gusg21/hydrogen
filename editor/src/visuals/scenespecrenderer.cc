//
// Created by angus.goucher on 9/24/2024.
//

#include "editor/visuals/scenespecrenderer.h"

#include <glad/glad.h>

#include "core/render/renderer.h"
#include "core/render/shader.h"
#include "editor/editor.h"
#include "editor/windows/sceneeditor.h"

static h_core::render::Vertex crossVerts[6] = {
    { h_core::math::Vector3 { -1, 0, 0 }, h_core::math::Vector3 {}, h_core::math::Vector2 {} },
    { h_core::math::Vector3 { 1, 0, 0 }, h_core::math::Vector3 {}, h_core::math::Vector2 {} },
    { h_core::math::Vector3 { 0, -1, 0 }, h_core::math::Vector3 {}, h_core::math::Vector2 {} },
    { h_core::math::Vector3 { 0, 1, 0 }, h_core::math::Vector3 {}, h_core::math::Vector2 {} },
    { h_core::math::Vector3 { 0, 0, -1 }, h_core::math::Vector3 {}, h_core::math::Vector2 {} },
    { h_core::math::Vector3 { 0, 0, 1 }, h_core::math::Vector3 {}, h_core::math::Vector2 {} },
};

static uint32_t crossIndices[9] = {
    0, 1, 0, // There and back again
    2, 3, 2,
    4, 5, 4
};

void h_editor::visuals::SceneSpecRenderer::init(h_editor::Editor* editor) {
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ::glEnable(GL_DEPTH_TEST);
    ::glDepthFunc(GL_LEQUAL);

    // VAO, VBO, EBO
    ::glGenVertexArrays(1, &m_vao);
    ::glBindVertexArray(m_vao);

    ::glGenBuffers(1, &m_vbo);
    ::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    ::glGenBuffers(1, &m_ebo);
    ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    h_core::render::Vertex::setUpVertexAttributes();

    ::glBindVertexArray(0);
    ::glBindBuffer(GL_ARRAY_BUFFER, 0);
    ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // FBO
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glGenTextures(1, &m_fbTex);
    glBindTexture(GL_TEXTURE_2D, m_fbTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCENEEDITOR_WIDTH, SCENEEDITOR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbTex, 0);

    // Shader
    uint32_t result = h_core::render::Renderer::loadProgram(
        &m_shader, "heditor/core/vs_viewport.glsl", "heditor/core/fs_viewport.glsl");
    if (result != 0) {
        HYLOG_WARN("SSR: Bad shader load!");
        return;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void h_editor::visuals::SceneSpecRenderer::render(h_editor::windows::SceneEditor* editor) {
    ::glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    ::glViewport(0, 0, SCENEEDITOR_WIDTH, SCENEEDITOR_HEIGHT);
    ::glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    ::glClear(GL_COLOR_BUFFER_BIT);
    ::glEnable(GL_DEPTH_TEST);
    ::glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    h_core::math::Mat4x4 projMatrix = h_core::math::Mat4x4::getProjMatrix(
        (fovY / 180.f) * static_cast<float>(M_PI),
        static_cast<float>(SCENEEDITOR_WIDTH) / static_cast<float>(SCENEEDITOR_HEIGHT), 0.01f, 100.f);
    h_core::math::Mat4x4 viewMatrix = h_core::math::Mat4x4::lookAtMat(cameraPosition, cameraPosition + cameraDirection);
    h_core::math::Mat4x4 viewProj = h_core::math::Mat4x4::multiply(projMatrix, viewMatrix);

    m_shader.use();
    m_shader.setMat4("uni_viewProjectionMatrix", viewProj);

    ::glBindVertexArray(m_vao);

    for (uint32_t actorIndex = 0; actorIndex < editor->getActorCount(); actorIndex++) {
        h_core::Transform transform = editor->getActorTransformAtIndex(actorIndex);

        m_shader.setMat4(
            "uni_modelMatrix",
            h_core::math::Mat4x4::createTransformMatrix(transform.position, transform.rotation, transform.scale));

        bool drawPoint = true;

        if (editor->actorHasModel(actorIndex)) {
            h_core::render::ModelAsset* model = editor->getActorModelAtIndex(actorIndex);

            if (model != nullptr) {
                if (model->getMeshCount() != 0) {
                    for (uint32_t meshIndex = 0; meshIndex < model->getMeshCount(); meshIndex++) {
                        h_core::render::Mesh* mesh = &model->getMeshes()[meshIndex];

                        ::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
                        ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
                        mesh->uploadDataToGPU(h_core::render::MeshAccessType::DYNAMIC);

                        GLenum glElementType;
                        switch (mesh->meshIndexType) {
                            case h_core::render::MeshIndexType::BYTE:
                                glElementType = GL_UNSIGNED_BYTE;
                                break;
                            case h_core::render::MeshIndexType::SHORT:
                                glElementType = GL_UNSIGNED_SHORT;
                                break;
                            case h_core::render::MeshIndexType::INT:
                                glElementType = GL_UNSIGNED_INT;
                                break;
                            default:
                                glElementType =
                                    GL_UNSIGNED_INT;  // clion was giving a warning about glElementType, it was
                                // annoying. I wrote a useless default
                        }

                        ::glDrawElements(GL_TRIANGLES, mesh->numIndices, glElementType, nullptr);
                    }

                    drawPoint = false;
                }
            }
        }

        if (drawPoint) {
            ::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

            ::glBufferData(GL_ARRAY_BUFFER, sizeof(h_core::render::Vertex) * 6, crossVerts, GL_DYNAMIC_DRAW);
            ::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 9, crossIndices, GL_DYNAMIC_DRAW);

            ::glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr);
        }

    }


    ::glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ::glBindVertexArray(0);

    ::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

ImTextureID h_editor::visuals::SceneSpecRenderer::getTexture() {
    return reinterpret_cast<ImTextureID>(m_fbTex);
}