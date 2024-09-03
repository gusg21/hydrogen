//
// Created by Angus Goucher on 9/2/2024.
//

#include "core/input/keyinputactionsource.h"

float h_core::input::KeyInputActionSource::getValue(h_core::input::Input* input) {
    return input->isKeyDown(scanCode) ? 1.f : 0.f;
}

const char* h_core::input::KeyInputActionSource::getName() const {
    return "KEY";
}
