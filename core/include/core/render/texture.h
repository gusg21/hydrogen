//
// Created by xavier.olmstead on 9/18/2024.
// Used ewinebrenner texture loading code on https://github.com/ewinebrenner/gpr300-sp24-template
// Direct path: https://github.com/ewinebrenner/gpr300-sp24-template/blob/main/core/ew/texture.h
//
#pragma once

#include <glad/glad.h>

#include <cstdint>
#include <string>
#include <vector>

#define TEXTURE_FAILED_TO_LOAD 1

namespace h_core {
namespace render {
enum class Filter { NEAREST, LINEAR };

enum class WrapMode { REPEAT, MIRRORED_REPEAT, CLAMP_TO_EDGE, CLAMP_TO_BORDER };

class Texture {
  public:
    Texture() = default;

    uint32_t textureId = UINT32_MAX;

    void init(
        const std::vector<unsigned char>& data, uint32_t width, uint32_t height, uint32_t componentCount,
        Filter magFilter, Filter minFilter, WrapMode wrapU, WrapMode wrapV);

    // File path statics
    static uint32_t loadTexture(
        uint32_t& out_texture, std::string filePath, GLint wrapMode, GLint minFilter, GLint magFilter, bool mipmap);
    static uint32_t loadTexture(uint32_t& out_texture, std::string filePath);

  private:
};
}  // namespace render
}  // namespace h_core