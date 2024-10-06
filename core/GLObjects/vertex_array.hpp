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
        if (m_UsedAttributeIndices.find(attributeIndex) !=
            m_UsedAttributeIndices.end()) {
            throw std::invalid_argument("Error: Attribute index " +
                                        std::to_string(attributeIndex) +
                                        " is already in use.");
        }

        bind();
        vb.bind();

        GLuint offset = 0;

        const auto &elements = layout.getElements();
        for (const auto &element : elements) {
            glEnableVertexAttribArray(attributeIndex);
            glVertexAttribPointer(attributeIndex, element.count, element.type,
                                  element.normalized, layout.getStride(),
                                  reinterpret_cast<const void *>(offset));
            offset += element.count *
                      VertexBufferElement::getSizeOfType(element.type);
        }

        vb.unbind();
        unbind();

        m_UsedAttributeIndices.insert(attributeIndex);
    }

    void bind() const { glBindVertexArray(m_RendererID); }

    void unbind() const { glBindVertexArray(0); }
};
