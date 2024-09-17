//
// Created by Angus Goucher on 9/17/2024.
//

#include "editor/platform/environment.h"

#include <stdlib.h>
#include <string>

std::string h_editor::platform::getEnvironmentVar(const std::string& key) {
    const char* val = getenv(
        key.c_str());  // Safe, just not guaranteed accessible across calls. Shut up MSVC!
                       // Source: https://stackoverflow.com/questions/48568707/getenv-function-may-be-unsafe-really
    return val == nullptr ? "" : std::string { val };
}