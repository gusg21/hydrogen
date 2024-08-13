#pragma once

#include <stdint.h>

namespace h_core {
class Event {
public:
    Event(uint32_t _type);

    uint32_t type = 0;
};
}  // namespace h_core
