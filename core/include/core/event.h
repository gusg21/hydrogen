#pragma once

#include <stdint.h>

namespace h_core {
class Event {
  public:
    Event() = default;

    /// @brief the type of this event
    uint32_t type = 0;

    /* Resized */
    /// @brief the new (resized) window width
    uint32_t newWindowWidth = 0;
    /// @brief the new (resized) window height
    uint32_t newWindowHeight = 0;

    /* Mouse Motion */
    uint32_t mouseX = 0;
    uint32_t mouseY = 0;
    int32_t mouseDx = 0;
    int32_t mouseDy = 0;
};
}  // namespace h_core
