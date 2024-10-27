#pragma once

#include "Common/util.hpp"
#include "interpolation.hpp"

#include <assimp/anim.h>

#include <string>
#include <vector>

class Bone {
  public:
    Bone(int id, const std::string &name, const aiNodeAnim *channel)
        : m_Index(id), m_Name(name), m_LocalTransform(1.0f) {
        m_NumPositions = channel->mNumPositionKeys;
        for (int positionIndex = 0; positionIndex < m_NumPositions;
             ++positionIndex) {
            aiVector3D aiPosition =
                channel->mPositionKeys[positionIndex].mValue;
            float timeStamp = channel->mPositionKeys[positionIndex].mTime;
            KeyPosition data = {
                .position = AssimpGLMConversion::aiVector3DToGLM(aiPosition),
                .timeStamp = timeStamp,
            };
            m_Positions.push_back(data);
        }

        m_NumRotations = channel->mNumRotationKeys;
        for (int rotationIndex = 0; rotationIndex < m_NumRotations;
             ++rotationIndex) {
            aiQuaternion aiOrientation =
                channel->mRotationKeys[rotationIndex].mValue;
            float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
            KeyRotation data = {
                .orientation =
                    AssimpGLMConversion::aiQuaternionToGLM(aiOrientation),
                .timeStamp = timeStamp,
            };
            m_Rotations.push_back(data);
        }

        m_NumScalings = channel->mNumScalingKeys;
        for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex) {
            aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
            float timeStamp = channel->mScalingKeys[keyIndex].mTime;
            KeyScale data = {
                .scale = AssimpGLMConversion::aiVector3DToGLM(scale),
                .timeStamp = timeStamp,
            };
            m_Scales.push_back(data);
        }
    }

    void update(float animationTime) {
        glm::mat4 translation;
        auto positionIndex = getPositionIndex(animationTime);
        if (m_NumPositions == 1) {
            translation =
                glm::translate(glm::mat4(1.0f), m_Positions[0].position);
        } else {
            translation =
                interpolatePosition(animationTime, m_Positions[positionIndex],
                                    m_Positions[positionIndex + 1]);
        }

        glm::mat4 rotation;
        auto rotationIndex = getRotationIndex(animationTime);
        if (m_NumRotations == 1) {
            rotation = glm::toMat4(glm::normalize(m_Rotations[0].orientation));
        } else {
            rotation =
                interpolateRotation(animationTime, m_Rotations[rotationIndex],
                                    m_Rotations[rotationIndex + 1]);
        }

        glm::mat4 scale;
        auto scaleIndex = getScaleIndex(animationTime);
        if (m_NumScalings == 1) {
            scale = glm::scale(glm::mat4(1.0f), m_Scales[0].scale);
        } else {
            scale = interpolateScaling(animationTime, m_Scales[scaleIndex],
                                       m_Scales[scaleIndex + 1]);
        }
        m_LocalTransform = translation * rotation * scale;
    }

    int getBoneID() { return m_Index; }
    std::string getBoneName() const { return m_Name; }
    glm::mat4 getLocalTransform() { return m_LocalTransform; }

    int getPositionIndex(float animationTime) const {
        for (int i = 0; i < m_NumPositions - 1; ++i) {
            if (animationTime < m_Positions[i + 1].timeStamp)
                return i;
        }
        return m_NumPositions - 2;
    }

    int getRotationIndex(float animationTime) const {
        for (int i = 0; i < m_NumRotations - 1; ++i) {
            if (animationTime < m_Rotations[i + 1].timeStamp)
                return i;
        }
        return m_NumRotations - 2;
    }

    int getScaleIndex(float animationTime) const {
        for (int i = 0; i < m_NumScalings - 1; ++i) {
            if (animationTime < m_Scales[i + 1].timeStamp)
                return i;
        }
        return m_NumScalings - 2;
    }

    KeyPosition getPositions(float animationTime) {
        auto positionIndex =
            (animationTime == 0.0f) ? 0 : getPositionIndex(animationTime) + 1;
        return m_Positions[positionIndex];
    }

    KeyRotation getRotations(float animationTime) {
        auto rotationIndex =
            (animationTime == 0.0f) ? 0 : getRotationIndex(animationTime) + 1;
        return m_Rotations[rotationIndex];
    }

    KeyScale getScalings(float animationTime) {
        auto scaleIndex =
            (animationTime == 0.0f) ? 0 : getScaleIndex(animationTime) + 1;
        return m_Scales[scaleIndex];
    }

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
};
