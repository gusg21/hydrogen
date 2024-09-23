#pragma once

#include <string>
#include <cstdint>

#include "core/math/mat4x4.h"

namespace h_core {
namespace render {

struct Shader {
    // TODO: Abstract these handles away from OpenGL land
    uint32_t vertexShader, fragmentShader;
    uint32_t program;

    void use();
    void setMat4(const std::string& name, h_core::math::Mat4x4 matrix);
};
}  // namespace render
}  // namespace h_core