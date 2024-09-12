//
// Created by Angus Goucher on 9/7/2024.
//

#pragma once

#include "core/componentbitmask.h"

#define HYCOMPONENT(bitIndex) [[nodiscard]] constexpr static h_core::ComponentBitmask getMask() { return 1 << bitIndex; }

namespace h_core {
class Component {
  public:
    HYCOMPONENT(0);
};
}
