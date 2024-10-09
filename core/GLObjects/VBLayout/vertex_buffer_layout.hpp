#pragma once

#include <GL/glew.h>
#include <vector>

struct VertexBufferElement {
    GLuint type;
    GLuint count;
    GLubyte normalized;

    static GLuint getSizeOfType(GLuint type) {
        switch (type) {
        case GL_FLOAT:
            return sizeof(GLfloat);
        case GL_UNSIGNED_INT:
            return sizeof(GLuint);
        case GL_INT:
            return sizeof(GLint);
        case GL_UNSIGNED_BYTE:
            return sizeof(GLubyte);
        }
        return 0;
    }
};

template <typename T> constexpr bool t_always_false() { return false; }

class VertexBufferLayout {
  private:
    GLuint m_Stride;
    std::vector<VertexBufferElement> m_Elements;

  public:
    VertexBufferLayout() : m_Stride(0) {}

    template <typename T> void push(GLuint count);

    inline GLuint getStride() const { return m_Stride; }
    inline const auto &getElements() const { return m_Elements; }
};
