#pragma once

#include "vertex_buffer.hpp"
#include "vertex_buffer_layout.hpp"

class VertexArray {
  private:
    GLuint m_RendererID;
    GLuint m_AttributeIndex;

  public:
    VertexArray() : m_RendererID(0), m_AttributeIndex(0) {
        glGenVertexArrays(1, &m_RendererID);
    }

    ~VertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    VertexArray(VertexArray &&other) noexcept
        : m_RendererID(other.m_RendererID),
          m_AttributeIndex(other.m_AttributeIndex) {
        other.m_RendererID = 0;
        other.m_AttributeIndex = 0;
    }

    VertexArray &operator=(VertexArray &&other) noexcept {
        if (this != &other) {
            glDeleteVertexArrays(1, &m_RendererID);
            m_RendererID = other.m_RendererID;
            m_AttributeIndex = other.m_AttributeIndex;
            other.m_RendererID = 0;
            other.m_AttributeIndex = 0;
        }
        return *this;
    }

    template <typename T>
    void addBuffer(const VertexBuffer<T> &vb,
                   const VertexBufferLayout &layout) {
        bind();
        vb.bind();

        const auto &elements = layout.getElements();
        GLuint offset = 0;

        for (const auto &element : elements) {
            glEnableVertexAttribArray(m_AttributeIndex);
            glVertexAttribPointer(m_AttributeIndex, element.count, element.type,
                                  element.normalized, layout.getStride(),
                                  reinterpret_cast<const void *>(offset));
            offset += element.count *
                      VertexBufferElement::getSizeOfType(element.type);
            ++m_AttributeIndex;
        }

        vb.unbind();
        unbind();
    }

    void bind() const { glBindVertexArray(m_RendererID); }

    void unbind() const { glBindVertexArray(0); }
};
