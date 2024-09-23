//
// Created by Angus Goucher on 9/4/2024.
//

#ifndef HYCORE_HEADLESS

#pragma once

#include "renderer.h"

namespace h_core {
namespace render {
class Gles3Renderer : public h_core::render::Renderer {
  public:
    uint32_t init(h_core::RuntimeEngine* engine) override;
    void beginFrame() override;
    void draw() override;

  private:
    h_core::render::Shader m_shader {};
};
}
}

#endif