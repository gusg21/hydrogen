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
    KeyInputActionSource() = default;
    explicit KeyInputActionSource(SDL_Scancode scanCode) : scanCode(scanCode) {}

    float getValue(h_core::input::Input* input) override;

    SDL_Scancode scanCode = SDL_SCANCODE_0;
};
}
}