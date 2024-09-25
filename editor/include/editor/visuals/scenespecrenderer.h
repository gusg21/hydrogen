//
// Created by angus.goucher on 9/24/2024.
//

#pragma once

#include <imgui.h>

#include "core/math/color.h"
#include "core/math/vector3.h"
#include "core/render/shader.h"
#include "core/scenespecasset.h"
#include "editor/editor.h"

#define SCENEEDITOR_WIDTH 800
#define SCENEEDITOR_HEIGHT 600

namespace h_editor::windows {
class SceneEditor;
}

namespace h_editor {
namespace visuals {

struct SSRVertex {
    SSRVertex() = default;
    SSRVertex(h_core::math::Vector3 position, h_core::math::Color color) : position(position), color(color) {}

    h_core::math::Vector3 position {};
    h_core::math::Color color {};
};

class SceneSpecRenderer {
  public:
    SceneSpecRenderer() = default;

    void init(h_editor::Editor* editor);
    void render(h_editor::windows::SceneEditor* scene);
    ImTextureID getTexture();

    h_core::math::Vector3 cameraPosition { -1.f, 0.f, 0.f };
    h_core::math::Vector3 cameraDirection { 1.f, 0.f, 0.f };
    float fovY = 80.f;

  private:
    ImTextureID texture = nullptr;

    uint32_t m_vao = 0, m_vbo = 0, m_ebo = 0;
    uint32_t m_fbo = 0;
    uint32_t m_fbTex = 0;
    h_core::render::Shader m_shader {};
};

}  // namespace visuals
}  // namespace h_editor