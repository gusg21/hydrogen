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
    uint32_t newWidth = 0;
    /// @brief the new (resized) window height
    uint32_t newHeight = 0;
};
}  // namespace h_core
