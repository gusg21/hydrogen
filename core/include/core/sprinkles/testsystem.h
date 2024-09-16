#pragma once

#include "core/runtimesystem.h"
#include "core/transform.h"

namespace h_core {
namespace sprinkles {
class TestSystem : public RuntimeSystem {
  public:
    TestSystem() = default;

    uint32_t init(h_core::RuntimeEngine* engine) override;
    void process() override;

    HYSYSTEM(h_core::Transform::getMask());
};
}
}  // namespace h_core