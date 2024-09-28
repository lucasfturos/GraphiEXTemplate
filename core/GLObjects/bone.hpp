#pragma once

#include <assimp/anim.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>

struct KeyPosition {
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation {
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale {
    glm::vec3 scale;
    float timeStamp;
};

class Bone {
  private:
    int m_index;
    std::string m_name;
    glm::mat4 m_localTransform;

    int m_NumPositions;

  public:
    Bone(int id, const std::string &name, const aiNodeAnim *channel)
        : m_index(id), m_name(name), m_localTransform(1.0) {
        m_NumPositions = channel->mNumPositionKeys;
    }

    int getBoneID() { return m_index; }
    std::string getBoneName() const { return m_name; }
    glm::mat4 getLocalTransform() { return m_localTransform; }
};