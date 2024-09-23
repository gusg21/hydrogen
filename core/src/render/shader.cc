#include "core/render/shader.h"

#ifndef HYCORE_HEADLESS
#include "glad/glad.h"
#endif

void h_core::render::Shader::use() {
#ifndef HYCORE_HEADLESS
    glUseProgram(program);
#endif
}

void h_core::render::Shader::setMat4(
    const std::string& name, h_core::math::Mat4x4 matrix) {
#ifndef HYCORE_HEADLESS
    glUniformMatrix4fv(
        glGetUniformLocation(program, name.c_str()), 1, GL_FALSE,
        matrix.matrix);
#endif
}