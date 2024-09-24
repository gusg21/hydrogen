//
// Created by angus.goucher on 9/23/2024.
//

#pragma once

#include <cstdint>
#include <string>

namespace h_editor {
namespace platform {

uint32_t invokeBlocking(const std::string& command, char* args);

}
}