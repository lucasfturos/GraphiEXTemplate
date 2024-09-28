#pragma once

#include <assimp/anim.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <vector>

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
    int m_Index;
    std::string m_Name;
    glm::mat4 m_LocalTransform;

    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
    std::vector<KeyScale> m_Scales;

    int m_NumPositions;
    int m_NumRotations;
    int m_NumScalings;

  public:
    Bone(int id, const std::string &name, const aiNodeAnim *channel)
        : m_Index(id), m_Name(name), m_LocalTransform(1.0f) {
        m_NumPositions = channel->mNumPositionKeys;
        for (int i = 0; i < m_NumPositions; ++i) {
            aiVector3D aiPosition = channel->mPositionKeys[i].mValue;
            float timeStamp = channel->mPositionKeys[i].mTime;
            KeyPosition keyPos = {
                glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z), timeStamp};
            m_Positions.push_back(keyPos);
        }

        m_NumRotations = channel->mNumRotationKeys;
        for (int i = 0; i < m_NumRotations; ++i) {
            aiQuaternion aiRotation = channel->mRotationKeys[i].mValue;
            float timeStamp = channel->mRotationKeys[i].mTime;
            KeyRotation keyRot = {glm::quat(aiRotation.w, aiRotation.x,
                                            aiRotation.y, aiRotation.z),
                                  timeStamp};
            m_Rotations.push_back(keyRot);
        }

        m_NumScalings = channel->mNumScalingKeys;
        for (int i = 0; i < m_NumScalings; ++i) {
            aiVector3D aiScale = channel->mScalingKeys[i].mValue;
            float timeStamp = channel->mScalingKeys[i].mTime;
            KeyScale keyScale = {glm::vec3(aiScale.x, aiScale.y, aiScale.z),
                                 timeStamp};
            m_Scales.push_back(keyScale);
        }
    }

    void update(float animationTime) {
        glm::mat4 translation = interpolatePosition(animationTime);
        glm::mat4 rotation = interpolateRotation(animationTime);
        glm::mat4 scale = interpolateScaling(animationTime);
        m_LocalTransform = translation * rotation * scale;
    }

    int getBoneID() const { return m_Index; }
    const std::string &getBoneName() const { return m_Name; }
    glm::mat4 getLocalTransform() const { return m_LocalTransform; }

  private:
    glm::mat4 interpolatePosition(float animationTime) {
        if (m_NumPositions == 1) {
            return glm::translate(glm::mat4(1.0f), m_Positions[0].position);
        }

        int p0Index = getPositionIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor =
            getScaleFactor(m_Positions[p0Index].timeStamp,
                           m_Positions[p1Index].timeStamp, animationTime);
        glm::vec3 finalPosition =
            glm::mix(m_Positions[p0Index].position,
                     m_Positions[p1Index].position, scaleFactor);
        return glm::translate(glm::mat4(1.0f), finalPosition);
    }

    glm::mat4 interpolateRotation(float animationTime) {
        if (m_NumRotations == 1) {
            return glm::toMat4(glm::normalize(m_Rotations[0].orientation));
        }

        int p0Index = getRotationIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor =
            getScaleFactor(m_Rotations[p0Index].timeStamp,
                           m_Rotations[p1Index].timeStamp, animationTime);
        glm::quat finalRotation =
            glm::slerp(m_Rotations[p0Index].orientation,
                       m_Rotations[p1Index].orientation, scaleFactor);
        return glm::toMat4(glm::normalize(finalRotation));
    }

    glm::mat4 interpolateScaling(float animationTime) {
        if (m_NumScalings == 1)
            return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);

        int p0Index = getScaleIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor =
            getScaleFactor(m_Scales[p0Index].timeStamp,
                           m_Scales[p1Index].timeStamp, animationTime);
        glm::vec3 finalScale = glm::mix(m_Scales[p0Index].scale,
                                        m_Scales[p1Index].scale, scaleFactor);
        return glm::scale(glm::mat4(1.0f), finalScale);
    }

    int getPositionIndex(float animationTime) const {
        for (int i = 0; i < m_NumPositions - 1; ++i) {
            if (animationTime < m_Positions[i + 1].timeStamp)
                return i;
        }
        return 0;
    }

    int getRotationIndex(float animationTime) const {
        for (int i = 0; i < m_NumRotations - 1; ++i) {
            if (animationTime < m_Rotations[i + 1].timeStamp)
                return i;
        }
        return 0;
    }

    int getScaleIndex(float animationTime) const {
        for (int i = 0; i < m_NumScalings - 1; ++i) {
            if (animationTime < m_Scales[i + 1].timeStamp)
                return i;
        }
        return 0;
    }

    float getScaleFactor(float lastTimeStamp, float nextTimeStamp,
                         float animationTime) const {
        float scaleFactor =
            (animationTime - lastTimeStamp) / (nextTimeStamp - lastTimeStamp);
        return scaleFactor;
    }
};
