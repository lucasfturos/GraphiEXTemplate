#pragma once

#include "VBLayout/vertex_buffer_layout.hpp"
#include "vertex_buffer.hpp"

#include <set>
#include <stdexcept>

class VertexArray {
  private:
    GLuint m_RendererID;
    std::set<GLuint> m_UsedAttributeIndices;

  public:
    VertexArray() : m_RendererID(0) { glGenVertexArrays(1, &m_RendererID); }

    ~VertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    VertexArray(VertexArray &&other) noexcept
        : m_RendererID(other.m_RendererID),
          m_UsedAttributeIndices(std::move(other.m_UsedAttributeIndices)) {
        other.m_RendererID = 0;
    }

    VertexArray &operator=(VertexArray &&other) noexcept {
        if (this != &other) {
            glDeleteVertexArrays(1, &m_RendererID);
            m_RendererID = other.m_RendererID;
            m_UsedAttributeIndices = std::move(other.m_UsedAttributeIndices);
            other.m_RendererID = 0;
        }
        return *this;
    }

    template <typename T>
    void addBuffer(const VertexBuffer<T> &vb, const VertexBufferLayout &layout,
                   GLuint attributeIndex) {
        bind();
        vb.bind();

        GLuint offset = 0;
        const auto &elements = layout.getElements();

        for (const auto &element : elements) {
            glEnableVertexAttribArray(attributeIndex);

            if (element.type == GL_INT || element.type == GL_UNSIGNED_INT) {
                glVertexAttribIPointer(attributeIndex, element.count,
                                       element.type, layout.getStride(),
                                       reinterpret_cast<const void *>(offset));
            } else {
                glVertexAttribPointer(attributeIndex, element.count,
                                      element.type, element.normalized,
                                      layout.getStride(),
                                      reinterpret_cast<const void *>(offset));
            }

            offset += element.count *
                      VertexBufferElement::getSizeOfType(element.type);
        }

        vb.unbind();
        unbind();
    }

    void bind() const { glBindVertexArray(m_RendererID); }

    void unbind() const { glBindVertexArray(0); }
};
