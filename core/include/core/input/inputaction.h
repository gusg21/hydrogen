//
// Created by Angus Goucher on 9/2/2024.
//

#pragma once

#include <string>
#include <vector>

#include "core/input/inputactionsource.h"

namespace h_core {
namespace input {
class InputAction {
  public:
    std::string name {};
    std::vector<h_core::input::InputActionSource*> sources {};
    float lastFrameValue = 0.f;

    float getValue(h_core::input::Input* input);
};
}
}