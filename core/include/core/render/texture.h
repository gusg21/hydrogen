//
// Created by xavier.olmstead on 9/18/2024.
// Used ewinebrenner texture loading code on https://github.com/ewinebrenner/gpr300-sp24-template
// Direct path: https://github.com/ewinebrenner/gpr300-sp24-template/blob/main/core/ew/texture.h
//
#pragma once
#include <../../../external/glad/include/glad/glad.h>


#include <cstdint>
#include <string>

#define TEXTURE_FAILED_TO_LOAD 1

namespace h_core {
namespace render {
class Texture {
public:
    Texture() = default;

    static uint32_t loadTexture(uint32_t& out_texture, std::string filePath, GLint wrapMode, GLint minFilter, GLint magFilter, bool mipmap);
    static uint32_t loadTexture(uint32_t& out_texture, std::string filePath);

private:
};
}
}