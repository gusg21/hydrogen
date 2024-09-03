//
// Created by Angus Goucher on 9/2/2024.
//
#pragma once

#include "core/input/input.h"
#include "core/input/inputactionsource.h"

namespace h_core {
namespace input {
class KeyInputActionSource : public InputActionSource {
  public:
    KeyInputActionSource() {
        type = InputActionSourceType::KEY;
    };
    explicit KeyInputActionSource(SDL_Scancode scanCode) : scanCode(scanCode) {
        type = InputActionSourceType::KEY;
    }

    float getValue(h_core::input::Input* input) override;
    [[nodiscard]] const char* getName() const override;

    SDL_Scancode scanCode = SDL_SCANCODE_0;
};
}
}