#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "core/math/mat4x4.h"

namespace h_core {
namespace systems {

struct Shader {
    GLuint vertexShader, fragmentShader;
    GLuint program;

    void use();
    void setMat4(std::string name, h_core::math::Mat4x4 matrix);
    void setMat4Glm(std::string name, glm::mat4 matrix);
};
}  // namespace systems
}  // namespace h_core