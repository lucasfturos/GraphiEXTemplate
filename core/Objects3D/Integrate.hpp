#pragma once

#include <GL/glew.h>
#include <cmath>
#include <functional>
#include <glm/glm.hpp>
#include <vector>

class Integrate {
  public:
    Integrate(std::vector<std::function<GLfloat(GLfloat, GLfloat)>> functions,
              GLfloat a, GLfloat b, GLfloat c, GLfloat d, int n, int m)
        : m_Functions(std::move(functions)), m_A(a), m_B(b), m_C(c), m_D(d),
          m_N(n), m_M(m), m_Vertices(genVertices()), m_Faces(genFaces()) {}

    const auto &getFaces() const { return m_Faces; }
    const auto &getVertices() const { return m_Vertices; }

  private:
    std::vector<glm::vec3> genVertices() {
        std::vector<glm::vec3> vertices;
        GLfloat dx = (m_B - m_A) / m_N;
        GLfloat dy = (m_D - m_C) / m_M;

        for (int i = 0; i <= m_N; ++i) {
            GLfloat u = m_A + i * dx;

            for (int j = 0; j <= m_M; ++j) {
                GLfloat v = m_C + j * dy;

                GLfloat x = m_Functions[0](u, v);
                GLfloat y = m_Functions[1](u, v);
                GLfloat z = m_Functions[2](u, v);

                vertices.push_back(glm::vec3(x, y, z));
            }
        }
        return vertices;
    }

    std::vector<GLuint> genFaces() {
        std::vector<GLuint> faces;

        for (int i = 0; i < m_N; ++i) {
            for (int j = 0; j < m_M; ++j) {
                GLuint topLeft = i * (m_M + 1) + j;
                GLuint bottomLeft = (i + 1) * (m_M + 1) + j;
                GLuint topRight = i * (m_M + 1) + (j + 1);
                GLuint bottomRight = (i + 1) * (m_M + 1) + (j + 1);

                faces.push_back(topLeft);
                faces.push_back(bottomLeft);
                faces.push_back(topRight);

                faces.push_back(topRight);
                faces.push_back(bottomLeft);
                faces.push_back(bottomRight);
            }
        }
        return faces;
    }

    std::vector<std::function<GLfloat(GLfloat, GLfloat)>> m_Functions;
    GLfloat m_A, m_B, m_C, m_D;
    int m_N, m_M;

    std::vector<glm::vec3> m_Vertices;
    std::vector<GLuint> m_Faces;
};
