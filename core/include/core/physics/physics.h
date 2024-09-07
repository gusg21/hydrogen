//
// Created by xavier.olmstead on 9/7/2024.
//

#pragma once

#include "core/system.h"

namespace h_core {
namespace physics {
class Physics : public System {
  public:
    uint32_t init(h_core::Engine* engine) override;
    void endFrame() override;

  private:
};
}
}
