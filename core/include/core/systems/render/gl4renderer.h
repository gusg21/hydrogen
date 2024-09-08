//
// Created by Angus Goucher on 9/4/2024.
//

#pragma once

#include "core/systems/render/renderer.h"

namespace h_core {
namespace render {
class Gl4Renderer : public Renderer {
  public:
    uint32_t init(h_core::Engine* engine) override;
    void beginFrame() override;
    void draw() override;

  private:
    h_core::render::Shader m_shader {};
};

}
}