#pragma once

#include <glm/glm.hpp>
#include <stdexcept>
#include <vector>

class SoftBodyEffect {
  public:
    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        float reflection;
    };

    SoftBodyEffect() : m_TimeStep(0.0f), m_Gravity({0.0f, -9.81f, 0.0f}) {}

    void addParticle(const glm::vec3 &position, float reflection = 0.5f) {
        if (reflection < 0.0f || reflection > 1.0f)
            throw std::invalid_argument(
                "Reflection must be between 0.0 and 1.0.");

        m_Particles.push_back(
            {position, glm::vec3(0.0f), glm::vec3(0.0f), reflection});
    }

    void update() {
        applyGravity();
        handleCollisions();
    }

    void setTimeStep(float time = 0.01f) { m_TimeStep = time; }
    void setBoundingBox(const glm::vec3 &min, const glm::vec3 &max) {
        m_BoundMin = min;
        m_BoundMax = max;
    }

    std::vector<glm::vec3> getUpdatedVertices() {
        std::vector<glm::vec3> vertices;
        for (const auto &p : m_Particles) {
            vertices.push_back(p.position);
        }
        return vertices;
    }

  private:
    std::vector<Particle> m_Particles;

    float m_TimeStep;
    glm::vec3 m_Gravity;

    glm::vec3 m_BoundMin;
    glm::vec3 m_BoundMax;

    void applyGravity() {
        for (auto &particle : m_Particles) {
            particle.acceleration = m_Gravity;
            particle.velocity += particle.acceleration * m_TimeStep;
            particle.position += particle.velocity * m_TimeStep;
        }
    }

    void handleCollisions() {
        for (auto &particle : m_Particles) {
            for (int i = 0; i < 3; ++i) {
                if (particle.position[i] < m_BoundMin[i]) {
                    particle.position[i] = m_BoundMin[i] + 0.01f;
                    particle.velocity[i] *= -particle.reflection;
                }
                if (particle.position[i] > m_BoundMax[i]) {
                    particle.position[i] = m_BoundMax[i] - 0.01f;
                    particle.velocity[i] *= -particle.reflection;
                }
            }
        }
    }
};
