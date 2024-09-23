//
// Created by xavier.olmstead on 9/18/2024.
//
#include "core/render/texture.h"

#include <cassert>
#include <string.h>

#include "core/log.h"

#define WRAP_MODE_TO_GL_WRAP_MODE(wrap, glWrap) \
    GLint glWrap;                               \
    switch (wrap) {                             \
        case WrapMode::REPEAT:                  \
            glWrap = GL_REPEAT;                 \
            break;                              \
        case WrapMode::MIRRORED_REPEAT:         \
            glWrap = GL_MIRRORED_REPEAT;        \
            break;                              \
        case WrapMode::CLAMP_TO_EDGE:           \
            glWrap = GL_CLAMP_TO_EDGE;          \
            break;                              \
        case WrapMode::CLAMP_TO_BORDER:         \
            glWrap = GL_CLAMP_TO_BORDER;        \
            break;                              \
                                                \
        default:                                \
            glWrap = GL_REPEAT;                 \
            break;                              \
    }

#define COMPONENT_COUNT_TO_FORMAT(componentCount, format) \
    uint32_t format;                                      \
    switch (componentCount) {                             \
        case 1:                                           \
            format = GL_RED;                              \
            break;                                        \
        case 2:                                           \
            format = GL_RG;                               \
            break;                                        \
        case 3:                                           \
            format = GL_RGB;                              \
            break;                                        \
        default:                                          \
            format = GL_RGBA;                             \
            break;                                        \
    }


void h_core::render::Texture::init(
    const uint8_t* data, size_t dataSize, uint32_t width, uint32_t height, uint32_t componentCount, Filter magFilter, Filter minFilter,
    WrapMode wrapU, WrapMode wrapV) {
    this->m_data = new uint8_t[dataSize];
    memcpy(m_data, data, dataSize);
    this->m_dataSize = dataSize;
    this->m_width = width;
    this->m_height = height;
    this->m_componentCount = componentCount;
    this->m_magFilter = magFilter;
    this->m_minFilter = minFilter;
    this->m_wrapU = wrapU;
    this->m_wrapV = wrapV;
}

void h_core::render::Texture::addToPacked(uint8_t* _writeHead) {
    uint8_t* writeHead = _writeHead;
    uint8_t* originalHead = _writeHead;

    // width
    memcpy(writeHead, (uint8_t*)&m_width, sizeof(uint32_t) * 1);
    writeHead += sizeof(uint32_t) * 1;

    // height
    memcpy(writeHead, (uint8_t*)&m_height, sizeof(uint32_t) * 1);
    writeHead += sizeof(uint32_t) * 1;

    // comp ct
    memcpy(writeHead, (uint8_t*)&m_componentCount, sizeof(uint32_t) * 1);
    writeHead += sizeof(uint32_t) * 1;

    // mag filter
    memcpy(writeHead, (uint8_t*)&m_magFilter, sizeof(Filter) * 1);
    writeHead += sizeof(Filter) * 1;

    // min filter
    memcpy(writeHead, (uint8_t*)&m_minFilter, sizeof(Filter) * 1);
    writeHead += sizeof(Filter) * 1;

    // wrap U
    memcpy(writeHead, (uint8_t*)&m_wrapU, sizeof(WrapMode) * 1);
    writeHead += sizeof(WrapMode) * 1;

    // wrap V
    memcpy(writeHead, (uint8_t*)&m_wrapV, sizeof(WrapMode) * 1);
    writeHead += sizeof(WrapMode) * 1;

    // data size
    memcpy(writeHead, (uint8_t*)&m_dataSize, sizeof(size_t) * 1);
    writeHead += sizeof(size_t) * 1;

    // data
    memcpy(writeHead, m_data, m_dataSize);
    writeHead += m_dataSize;

    assert(writeHead - originalHead == getPackedSize());
}

void h_core::render::Texture::readFromPacked(const uint8_t* _readHead) {
    const uint8_t* readHead = _readHead;

    // width
    m_width = *(uint32_t*)readHead;
    readHead += sizeof(uint32_t);

    // height
    m_height = *(uint32_t*)readHead;
    readHead += sizeof(uint32_t);

    // comp ct
    m_componentCount = *(uint32_t*)readHead;
    readHead += sizeof(uint32_t);

    // mag filter
    m_magFilter = *(Filter*)readHead;
    readHead += sizeof(Filter);

    // min filter
    m_minFilter = *(Filter*)readHead;
    readHead += sizeof(Filter);

    // wrap U
    m_wrapU = *(WrapMode*)readHead;
    readHead += sizeof(WrapMode);

    // wrap V
    m_wrapV = *(WrapMode*)readHead;
    readHead += sizeof(WrapMode);

    // data size
    m_dataSize = *(size_t*)readHead;
    readHead += sizeof(size_t);

    // data
    delete m_data;
    m_data = new uint8_t[m_dataSize];
    memcpy(m_data, readHead, m_dataSize);
    readHead += sizeof(uint8_t) * m_dataSize;

}

size_t h_core::render::Texture::getPackedSize() const {
    return sizeof(uint32_t) * 3 + sizeof(Filter) * 2 + sizeof(WrapMode) * 2 + sizeof(size_t) * 1 +
           sizeof(uint8_t) * m_dataSize;
}

void h_core::render::Texture::precompile() {
#ifndef HYCORE_HEADLESS
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    COMPONENT_COUNT_TO_FORMAT(m_componentCount, format);
    glTexImage2D(
        GL_TEXTURE_2D, 0, static_cast<GLint>(format), static_cast<int32_t>(m_width), static_cast<int32_t>(m_height), 0,
        format, GL_UNSIGNED_BYTE, m_data);

    WRAP_MODE_TO_GL_WRAP_MODE(m_wrapU, wrapModeU);
    WRAP_MODE_TO_GL_WRAP_MODE(m_wrapV, wrapModeV);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModeU);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModeV);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter == Filter::LINEAR ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter == Filter::LINEAR ? GL_LINEAR : GL_NEAREST);

    // Black border by default
    float borderColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // TODO: Add mipmap control
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
#else
    HYLOG_INFO("TEXTURE: Can't precompile (headless)");
#endif
}
