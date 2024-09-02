//
// Created by Angus Goucher on 9/2/2024.
//

#pragma once

namespace h_core {
namespace input {
class Input;

enum class InputActionSourceType { KEY };

class InputActionSource {
  public:
    h_core::input::InputActionSourceType type = h_core::input::InputActionSourceType::KEY;

    virtual float getValue(Input* input) = 0;
};
}
}
