//
// Created by Angus Goucher on 9/2/2024.
//

#pragma once

#include "SDL.h"

#include "core/input/inputactionsource.h"

namespace h_core {
namespace project {
struct ActionSourceEntry {
    ActionSourceEntry() = default;

    h_core::input::InputActionSourceType type =
        h_core::input::InputActionSourceType::KEY;
    union {
        SDL_Scancode scanCode;
    } value {};
};

struct ProjectActionEntry {
    ProjectActionEntry() = default;

    std::string name {};
    std::vector<h_core::project::ActionSourceEntry> sources {};
};
}  // namespace project
}  // namespace h_core
