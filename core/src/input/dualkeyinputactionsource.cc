//
// Created by Angus Goucher on 9/2/2024.
//
#include "core/input/input.h"
#include "core/input/dualkeyinputactionsource.h"

float h_core::input::DualKeyInputActionSource::getValue(
    h_core::input::Input* input) {
    float pos = input->isKeyDown(positiveScanCode) ? 1.f : 0.f;
    float neg = input->isKeyDown(negativeScanCode) ? 1.f : 0.f;
    return pos - neg;
}

const char* h_core::input::DualKeyInputActionSource::getName() const {
    return "DUAL KEY";
}
