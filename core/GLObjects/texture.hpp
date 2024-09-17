#pragma once

#include <GL/glew.h>
#include <stdexcept>
#include <vector>

class Texture {
  private:
    GLuint m_RendererID;
    GLenum m_TextureType;

  public:
    Texture(const std::string &imagePath, GLenum textureType);
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
            other.m_RendererID = 0;
        }
        return *this;
    }

    void bind(unsigned int slot = 0) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(m_TextureType, m_RendererID);
    }
    void unbind() const { glBindTexture(m_TextureType, 0); }

    void updateData(const std::vector<float> &data, int width, int height,
                    int depth, GLenum format, GLenum type) const;
};
