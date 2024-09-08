//
// Created by Angus Goucher on 9/7/2024.
//

#include "imgui.h"

#include <stdint.h>

namespace h_core {
namespace widgets {
void bitField(const char* label, uint32_t* bits) {
//    bool bitOn[32];
//    uint32_t newBits = *bits;
//    if (ImGui::CollapsingHeader(label)) {
//        for (uint32_t bitIndex = 0; bitIndex < 32; bitIndex++) {
//            uint32_t mask = (1 << bitIndex);
//            bitOn[bitIndex] = (newBits & mask) > 0;
//            ImGui::Text("%2d", bitIndex);
//            ImGui::SameLine();
//            ImGui::Checkbox("", &bitOn[bitIndex]);
//            if (bitIndex % 8 != 7) { ImGui::SameLine(); }
//            if (bitOn[bitIndex]) { newBits |= mask; }
//            else { newBits &= ~mask; }
//        }
//    }

//    *bits = newBits;
}
}  // namespace widgets
}  // namespace h_core