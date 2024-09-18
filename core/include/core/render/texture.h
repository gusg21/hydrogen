//
// Created by xavier.olmstead on 9/18/2024.
// Used ewinebrenner texture loading code on https://github.com/ewinebrenner/gpr300-sp24-template
// Direct path: https://github.com/ewinebrenner/gpr300-sp24-template/blob/main/core/ew/texture.h
//
#pragma once
#include <../../../external/glad/include/glad/glad.h>


#include <cstdint>
#include <string>

namespace render {
class Texture {
public:
    Texture() = default;

    uint32_t loadTexture(std::string filePath, GLint wrapMode, GLint minFilter, GLint magFilter, bool mipmap);
    uint32_t loadTexture(std::string filePath);

private:
};
}
