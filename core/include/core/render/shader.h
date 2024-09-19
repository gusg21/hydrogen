#pragma once

#include <string>

#include "glad/glad.h"

#include "core/math/mat4x4.h"

namespace h_core {
namespace render {

struct Shader {
    GLuint vertexShader, fragmentShader;
    GLuint program;

    void use();
    void setMat4(const std::string& name, h_core::math::Mat4x4 matrix);
    void setInt(const std::string& name, int32_t value);
};
}  // namespace render
}  // namespace h_core