#include "core/render/shader.h"

#include "glm/gtc/type_ptr.hpp"

void h_core::render::Shader::use() {
    glUseProgram(program);
}

void h_core::render::Shader::setMat4(
    std::string name, h_core::math::Mat4x4 matrix) {
    glUniformMatrix4fv(
        glGetUniformLocation(program, name.c_str()), 1, GL_FALSE,
        matrix.matrix);
}

void h_core::render::Shader::setMat4Glm(std::string name, glm::mat4 matrix) {
    glUniformMatrix4fv(
        glGetUniformLocation(program, name.c_str()), 1, GL_FALSE,
        glm::value_ptr(matrix));
}