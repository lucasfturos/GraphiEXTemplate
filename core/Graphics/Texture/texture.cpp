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
    : m_RendererID(0), m_TextureType(GL_TEXTURE_2D), m_Width(0), m_Height(0),
      m_Channels(0), m_ImageData(nullptr), m_Format(0), m_Type(0) {
    stbi_set_flip_vertically_on_load(false);
    m_ImageData =
        stbi_load(imagePath.c_str(), &m_Width, &m_Height, &m_Channels, 0);

    if (!m_ImageData) {
        throw std::runtime_error("Failed to load texture image!");
    }

    glGenTextures(1, &m_RendererID);
    glBindTexture(m_TextureType, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLenum m_Format = (m_Channels == 4) ? GL_RGBA : GL_RGB;
    m_Type = GL_UNSIGNED_BYTE;
    glTexImage2D(GL_TEXTURE_2D, 0, m_Format, m_Width, m_Height, 0, m_Format,
                 m_Type, m_ImageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    unbind();

    if (m_ImageData) {
        stbi_image_free(m_ImageData);
    }
}

Texture::~Texture() { glDeleteTextures(1, &m_RendererID); }
