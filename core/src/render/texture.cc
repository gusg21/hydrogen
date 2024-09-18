//
// Created by xavier.olmstead on 9/18/2024.
//
#include "core/render/texture.h"

#include <stb_image.h>

#include "core/log.h"

static uint32_t getTextureFormat(int32_t componentNumber) {
    switch (componentNumber) {
        case 1: return GL_RED;
            break;
        case 2: return GL_RG;
            break;
        case 3: return GL_RGB;
            break;
        default: return GL_RGBA;
    }
}


uint32_t h_core::render::Texture::loadTexture(uint32_t& out_texture,
    std::string filePath, GLint wrapMode, GLint minFilter, GLint magFilter, bool mipmap) {
    stbi_set_flip_vertically_on_load(true);

    int32_t width, height, numComponents;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &numComponents, 0);

    if (data == nullptr) {
        HYLOG_ERROR("RENDERING: TEXTURE: failed to load texture from %s", filePath);
        stbi_image_free(data);
        return TEXTURE_FAILED_TO_LOAD; //TODO: write error message for this
    }

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    uint32_t format = getTextureFormat(numComponents);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    //Black border by default
    float borderColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    if (mipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    out_texture = texture;
}

uint32_t h_core::render::Texture::loadTexture(uint32_t& out_texture, std::string filePath) {
    loadTexture(out_texture, filePath, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true);
}