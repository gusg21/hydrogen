//
// Created by Angus Goucher on 9/2/2024.
//

#pragma once

#include "SDL.h"

#include "core/input/input.h"
#include "core/input/inputactionsource.h"

namespace h_core {
namespace input {
class DualKeyInputActionSource : public InputActionSource {
  public:
    DualKeyInputActionSource() {
        type = InputActionSourceType::DUAL_KEY;
    }
    DualKeyInputActionSource(SDL_Scancode positiveScanCode, SDL_Scancode negativeScanCode)
        : positiveScanCode(positiveScanCode), negativeScanCode(negativeScanCode) {
        type = InputActionSourceType::DUAL_KEY;
    }

    float getValue(h_core::input::Input* input) override;
    [[nodiscard]] const char* getName() const override;

    SDL_Scancode positiveScanCode = SDL_SCANCODE_0;
    SDL_Scancode negativeScanCode = SDL_SCANCODE_0;
};
}  // namespace input
}  // namespace h_core