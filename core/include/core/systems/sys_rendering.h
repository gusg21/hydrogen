#pragma once

#include "bgfx/bgfx.h"
#include "core/system.h"

namespace h_core {
namespace system {
class Rendering : public System {
  public:
    Rendering() = default;

    uint32_t init();
    void process();
    h_core::ComponentBitmask getMask();

    uint32_t initFromWindow(uint32_t width, uint32_t height, void* nwh);

  private:
    h_core::math::Vector3 cameraPosition = h_core::math::Vector3(0);
    h_core::math::Vector3 cameraDirection = h_core::math::Vector3(0);

    float m_width = 0;
    float m_height = 0;
    bgfx::ProgramHandle m_program;
};
}  // namespace system
}  // namespace h_core