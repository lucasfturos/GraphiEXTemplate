#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(int width, int height, int depth, GLenum format, GLenum type,
                 GLenum textureType)
    : m_RendererID(0), m_TextureType(textureType) {
    glGenTextures(1, &m_RendererID);
    glBindTexture(m_TextureType, m_RendererID);

    switch (m_TextureType) {
    case GL_TEXTURE_2D:
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type,
                     nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case GL_TEXTURE_3D:
        glTexImage3D(GL_TEXTURE_3D, 0, format, width, height, depth, 0, format,
                     type, nullptr);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    default:
        glDeleteTextures(1, &m_RendererID);
        m_RendererID = 0;
        throw std::invalid_argument("Unsupported texture type");
        break;
    }

    unbind();
}

Texture::Texture(const std::string &imagePath)
    : m_RendererID(0), m_TextureType(GL_TEXTURE_2D) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data =
        stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
        throw std::runtime_error("Failed to load texture image!");
    }

    glGenTextures(1, &m_RendererID);
    glBindTexture(m_TextureType, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    unbind();

    if (data) {
        stbi_image_free(data);
    }
}

Texture::~Texture() { glDeleteTextures(1, &m_RendererID); }

void Texture::updateData(const std::vector<float> &data, int width, int height,
                         int depth, GLenum format, GLenum type) const {
    bind();
    switch (m_TextureType) {
    case GL_TEXTURE_2D:
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type,
                        data.data());
        break;
    case GL_TEXTURE_3D:
        glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth, format,
                        type, data.data());
        break;
    default:
        break;
    }
    unbind();
}