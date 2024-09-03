//
// Created by Angus Goucher on 9/2/2024.
//

#pragma once

#include "SDL.h"

#include "core/input/inputactionsource.h"

namespace h_core {
namespace project {
struct ProjectActionSourceEntry {
    ProjectActionSourceEntry() = default;

    h_core::input::InputActionSourceType type =
        h_core::input::InputActionSourceType::KEY;
    union {
        SDL_Scancode scanCode;
        SDL_Scancode dualScanCodes[2];
    } value {};
};

struct ProjectActionEntry {
    ProjectActionEntry() = default;

    std::string name {};
    std::vector<h_core::project::ProjectActionSourceEntry> sources {};
};
}  // namespace project
}  // namespace h_core
