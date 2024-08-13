#pragma once

#include "bx/string.h"

namespace h_core {
class Project {
public:
    Project();

    bx::StringLiteral projectName = "Unnamed Project";
};
}  // namespace h_core
