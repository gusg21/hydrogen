#include "core/render/shader.h"

#ifndef HYCORE_HEADLESS
#include "glad/glad.h"
#endif

void h_core::render::Shader::use() {
#ifndef HYCORE_HEADLESS
    glUseProgram(program);
#endif
}

void h_core::render::Shader::setMat4(const std::string& name, h_core::math::Mat4x4 matrix) {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, matrix.matrix);
}

void h_core::render::Shader::setInt(const std::string& name, int32_t value) {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}