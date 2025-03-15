#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <vector>

/* Study Links
 * https://github.com/lscholte/ClothSimulation/
 * https://github.com/QuantamHD/OpenGL-Cloth-Simulation
 * https://github.com/likangning93/GPU_cloth
 * https://github.com/bailus/Cloth
 * https://www.gorillasun.de/blog/euler-and-verlet-integration-for-particle-physics/
 * https://pikuma.com/blog/verlet-integration-2d-cloth-physics-simulation
 * https://matthias-research.github.io/pages/tenMinutePhysics/09-xpbd.pdf
 * http://www.darwin3d.com/gdm1999.htm
 * https://carmencincotti.com/2022-08-22/the-distance-constraint-of-xpbd/
 */

class SoftBodyEffect {
  protected:
    const float eps = 1.0e-3f;

    struct Particle {
        glm::vec3 position;
        glm::vec3 oldPosition;
        glm::vec3 velocity;
        float mass;
    };

    struct Spring {
        int p1, p2;
        float restLength;
        float stiffness;
        float damping;
    };

  private:
    std::vector<Particle> m_Particles;
    std::vector<Spring> m_Springs;

    float m_TimeStep;
    glm::vec3 m_Gravity;
    glm::vec3 m_BoundMin;
    glm::vec3 m_BoundMax;

    void integrate() {
        for (auto &particle : m_Particles) {
            glm::vec3 prevPosition = particle.position;

            particle.position =
                (2.0f * particle.position - particle.oldPosition) +
                m_Gravity * (m_TimeStep * m_TimeStep);
            particle.velocity =
                (particle.position - particle.oldPosition) / m_TimeStep;

            particle.oldPosition = prevPosition;

            handleCollision(particle);
        }
    }

    void handleCollision(Particle &particle) {
        for (int i = 0; i < 3; ++i) {
            if (particle.position[i] < m_BoundMin[i]) {
                particle.position[i] = m_BoundMin[i] + 0.01f;
                particle.velocity[i] = 0.0f;
            }
            if (particle.position[i] > m_BoundMax[i]) {
                particle.position[i] = m_BoundMax[i] - 0.01f;
                particle.velocity[i] = 0.0f;
            }
        }
    }

  public:
    SoftBodyEffect() : m_TimeStep(0.01f), m_Gravity({0.0f, -9.81f, 0.0f}) {}

    void addParticle(const glm::vec3 &position, float mass = 1.0f) {
        m_Particles.push_back({position, position, glm::vec3(0.0f), mass});
    }

    void addSpring(int p1, int p2, float stiffness = 100.0f,
                   float damping = 0.5f) {
        float restLength =
            glm::distance(m_Particles[p1].position, m_Particles[p2].position);
        m_Springs.push_back({p1, p2, restLength, stiffness, damping});
    }

    void update() { integrate(); }

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
};
