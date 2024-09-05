//
// Created by Angus Goucher on 9/4/2024.
//

#include "core/systems/render/gl4renderer.h"

uint32_t h_core::render::Gl4Renderer::init(h_core::Engine* engine) {
    Renderer::init(engine);

    setRendererName("GL4 Renderer");
    setIsGles3(false);

    m_shader = h_core::render::Shader {};
    uint32_t shaderLoadResult =
        loadProgram(&m_shader, "hcore_assets/core/vs_default.glsl", "hcore_assets/core/fs_default.glsl");
    if (shaderLoadResult != 0) { return RENDERING_INIT_FAIL_BAD_PROGRAM; }

    return 0;
}
void h_core::render::Gl4Renderer::beginFrame() {
    m_shader.use();
    h_core::math::Mat4x4 viewMatrix = h_core::math::Mat4x4::lookAtMat(
        m_cameraPosition, h_core::math::Vector3::add(m_cameraPosition, m_cameraDirection));

    float aspect = (float)engine->getWidth() / (float)engine->getHeight();
    h_core::math::Mat4x4 projMatrix =
        h_core::math::Mat4x4::getProjMatrix((m_fovDegrees / 180.f) * (float)M_PI, aspect, m_nearZ, m_farZ);
    m_shader.setMat4("uni_viewProjectionMatrix", h_core::math::Mat4x4::multiply(projMatrix, viewMatrix));

    Renderer::beginFrame();
}

void h_core::render::Gl4Renderer::draw() {
    Renderer::draw();

    m_shader.setMat4("uni_modelMatrix", transform->getMatrix());

    GLenum glElementType;
    switch (meshComp->mesh->getMeshIndexType()) {
        case h_core::render::MeshIndexType::BYTE:
            glElementType = GL_UNSIGNED_BYTE;
            break;
        case h_core::render::MeshIndexType::SHORT:
            glElementType = GL_UNSIGNED_SHORT;
            break;
        case h_core::render::MeshIndexType::INT:
            glElementType = GL_UNSIGNED_INT;
            break;
    }

    ::glBindVertexArray(meshComp->mesh->getVertexAttributesHandle());
    ::glDrawElements(meshComp->mesh->getPrimitiveMode(), meshComp->mesh->getNumIndices(), glElementType, nullptr);
}
