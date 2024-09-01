#include "core/systems/render/shader.h"


void h_core::render::Shader::use() {
    glUseProgram(program);
}

void h_core::render::Shader::setMat4(
    const std::string& name, h_core::math::Mat4x4 matrix) {
    glUniformMatrix4fv(
        glGetUniformLocation(program, name.c_str()), 1, GL_FALSE,
        matrix.matrix);
}