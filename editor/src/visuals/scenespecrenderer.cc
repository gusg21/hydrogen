//
// Created by angus.goucher on 9/24/2024.
//

#include "editor/visuals/scenespecrenderer.h"

#include <glad/glad.h>

#include "core/render/renderer.h"
#include "core/render/shader.h"
#include "editor/editor.h"

void h_editor::visuals::SceneSpecRenderer::init(h_editor::Editor* editor) {
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ::glEnable(GL_DEPTH_TEST);
    ::glDepthFunc(GL_LEQUAL);
    ::glEnable(GL_CULL_FACE);
    ::glCullFace(GL_BACK);

    // VAO, VBO, EBO
    ::glGenVertexArrays(1, &m_vao);
    ::glBindVertexArray(m_vao);

    ::glGenBuffers(1, &m_vbo);
    ::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    ::glBufferData(
        GL_ARRAY_BUFFER, sizeof(SSRVertex) * 3,
        new SSRVertex[] {
            { h_core::math::Vector3 { -0.5, -0.5, 0 }, h_core::math::Color {} },
            { h_core::math::Vector3 { 0.5, -0.5, 0 }, h_core::math::Color {} },
            { h_core::math::Vector3 { 0, 0.5, 0 }, h_core::math::Color {} },
        },
        GL_STATIC_DRAW);

    ::glGenBuffers(1, &m_ebo);
    ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    ::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 3, new uint32_t[] { 0, 1, 2 }, GL_STATIC_DRAW);

    ::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SSRVertex), (const void*)offsetof(SSRVertex, position));
    ::glEnableVertexAttribArray(0);

    ::glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SSRVertex), (const void*)offsetof(SSRVertex, color));
    ::glEnableVertexAttribArray(1);

    ::glBindVertexArray(0);
    ::glBindBuffer(GL_ARRAY_BUFFER, 0);
    ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // FBO
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glGenTextures(1, &m_fbTex);
    glBindTexture(GL_TEXTURE_2D, m_fbTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

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
}

void h_editor::visuals::SceneSpecRenderer::render() {
    ::glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    ::glViewport(0, 0, 800, 600);
    ::glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    ::glClear(GL_COLOR_BUFFER_BIT);
    ::glEnable(GL_DEPTH_TEST);
    ::glPolygonMode(GL_FRONT, GL_LINE);
    ::glPolygonMode(GL_BACK, GL_LINE);

    h_core::math::Mat4x4 projMatrix = h_core::math::Mat4x4::getProjMatrix(60.f, 800.f / 600.f, 0.01f, 100.f);
    h_core::math::Mat4x4 viewMatrix =
        h_core::math::Mat4x4::lookAtMat(m_cameraPosition, m_cameraPosition + m_cameraDirection);
    h_core::math::Mat4x4 viewProj = h_core::math::Mat4x4::multiply(projMatrix, viewMatrix);
    m_shader.setMat4("uni_viewProjectionMatrix", viewProj);

    m_triangleRotation = h_core::math::Quaternion::rotate(m_triangleRotation, 0.01f, h_core::math::Vector3 { 0, 1, 0 });
    m_shader.setMat4(
        "uni_modelMatrix",
        h_core::math::Mat4x4::createTransformMatrix(
            h_core::math::Vector3 { 0, 0, 0 }, m_triangleRotation, h_core::math::Vector3 { 1, 1, 1 }));

    m_shader.use();
    ::glBindVertexArray(m_vao);
    ::glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

    ::glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ::glBindVertexArray(0);
}

ImTextureID h_editor::visuals::SceneSpecRenderer::getTexture() {
    return reinterpret_cast<ImTextureID>(m_fbTex);
}