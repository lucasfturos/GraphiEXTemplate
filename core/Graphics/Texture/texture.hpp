#pragma once

#include <GL/glew.h>
#include <stdexcept>
#include <vector>

class Texture {
  private:
    GLuint m_RendererID;
    GLenum m_TextureType;

    int m_Width;
    int m_Height;
    int m_Channels;
    unsigned char *m_ImageData;

    GLenum m_Format;
    GLenum m_Type;

  public:
    Texture(const std::string &imagePath);
    Texture(int width, int height, int depth, GLenum format, GLenum type,
            GLenum textureType);
    ~Texture();

    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    Texture(Texture &&other) noexcept : m_RendererID(other.m_RendererID) {
        other.m_RendererID = 0;
    }

    Texture &operator=(Texture &&other) noexcept {
        if (this != &other) {
            glDeleteTextures(1, &m_RendererID);
            m_RendererID = other.m_RendererID;
            m_TextureType = other.m_TextureType;
            m_Width = other.m_Width;
            m_Height = other.m_Height;
            m_ImageData = other.m_ImageData;

            m_Format = other.m_Format;
            m_Type = other.m_Type;
            other.m_RendererID = 0;
        }
        return *this;
    }

    int getWidth() const { return m_Width; }
    int getHeight() const { return m_Height; }
    int getChannels() const { return m_Channels; }
    unsigned char *getImageData() const { return m_ImageData; }

    GLenum getFormat() const { return m_Format; }
    GLenum getType() const { return m_Type; }

    void bind(unsigned int slot = 0) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(m_TextureType, m_RendererID);
    }
    void unbind() const { glBindTexture(m_TextureType, 0); }

    template <typename T>
    void updateData(const std::vector<T> &data, int width, int height,
                    int depth, GLenum format, GLenum type) const {
        bind();
        switch (m_TextureType) {
        case GL_TEXTURE_2D:
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type,
                            data.data());
            break;
        case GL_TEXTURE_3D:
            glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth,
                            format, type, data.data());
            break;
        default:
            break;
        }
        unbind();
    }
};
