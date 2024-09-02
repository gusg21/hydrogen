//
// Created by Angus Goucher on 9/2/2024.
//

#include "core/input/inputaction.h"

float h_core::input::InputAction::getValue(h_core::input::Input* input) {
    // Easy out if only one source
    if (sources.size() == 1) return sources[0]->getValue(input);

    // Find the input source w/ the largest magnitude (positive or negative)
    float max = 0.f;
    for (h_core::input::InputActionSource* source : sources) {
        float value = source->getValue(input);
        if (::abs(value) > ::abs(max)) {
            max = value;
        }
    }

    return max;
}
