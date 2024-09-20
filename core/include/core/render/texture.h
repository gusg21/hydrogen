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
        uint8_t* data, size_t dataSize, uint32_t width, uint32_t height, uint32_t componentCount, Filter magFilter, Filter minFilter,
        WrapMode wrapU, WrapMode wrapV);
    void precompile();
    [[nodiscard]] size_t getPackedSize() const;
    void addToPacked(uint8_t* _writeHead);
    void readFromPacked(const uint8_t* _readHead);

  private:
    uint8_t* m_data = nullptr;
    size_t m_dataSize = 0;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    uint32_t m_componentCount = 4;
    Filter m_magFilter = Filter::LINEAR;
    Filter m_minFilter = Filter::LINEAR;
    WrapMode m_wrapU = WrapMode::REPEAT;
    WrapMode m_wrapV = WrapMode::REPEAT;
};
}  // namespace render
}  // namespace h_core