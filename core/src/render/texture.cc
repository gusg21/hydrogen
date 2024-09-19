//
// Created by xavier.olmstead on 9/18/2024.
//
#include "core/render/texture.h"

#include <stb_image.h>

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
    const std::vector<unsigned char>& data, uint32_t width, uint32_t height, uint32_t componentCount, Filter magFilter,
    Filter minFilter, WrapMode wrapU, WrapMode wrapV) {

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    COMPONENT_COUNT_TO_FORMAT(componentCount, format);
    glTexImage2D(
        GL_TEXTURE_2D, 0, static_cast<GLint>(format), static_cast<int32_t>(width), static_cast<int32_t>(height), 0,
        format, GL_UNSIGNED_BYTE, data.data());

    WRAP_MODE_TO_GL_WRAP_MODE(wrapU, wrapModeU);
    WRAP_MODE_TO_GL_WRAP_MODE(wrapV, wrapModeV);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModeU);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModeV);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter == Filter::LINEAR ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter == Filter::LINEAR ? GL_LINEAR : GL_NEAREST);

    // Black border by default
    float borderColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // TODO: Add mipmap control
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t h_core::render::Texture::loadTexture(
    uint32_t& out_texture, std::string filePath, GLint wrapMode, GLint minFilter, GLint magFilter, bool mipmap) {
    stbi_set_flip_vertically_on_load(true);

    int32_t width, height, numComponents;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &numComponents, 0);

    if (data == nullptr) {
        HYLOG_ERROR("RENDERING: TEXTURE: failed to load texture from %s", filePath);
        stbi_image_free(data);
        return TEXTURE_FAILED_TO_LOAD;  // TODO: write error message for this
    }

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    COMPONENT_COUNT_TO_FORMAT(numComponents, format);
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    // Black border by default
    float borderColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    if (mipmap) { glGenerateMipmap(GL_TEXTURE_2D); }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    out_texture = texture;
}

uint32_t h_core::render::Texture::loadTexture(uint32_t& out_texture, std::string filePath) {
    return loadTexture(out_texture, filePath, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true);
}