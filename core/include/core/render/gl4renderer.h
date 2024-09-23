//
// Created by Angus Goucher on 9/4/2024.
//

#pragma once

#ifndef HYCORE_HEADLESS

#include "renderer.h"

namespace h_core {
namespace render {
class Gl4Renderer : public Renderer {
  public:
    uint32_t init(h_core::RuntimeEngine* engine) override;
    void beginFrame() override;
    void draw() override;

  private:
    h_core::render::Shader m_shader {};
};

}  // namespace render
}  // namespace h_core

#endif