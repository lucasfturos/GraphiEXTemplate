#pragma once

#include "Common/util.hpp"
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
  public:
    Bone(int id, const std::string &name, const aiNodeAnim *channel)
        : m_Index(id), m_Name(name), m_LocalTransform(1.0f) {
        m_NumPositions = channel->mNumPositionKeys;
        for (int positionIndex = 0; positionIndex < m_NumPositions;
             ++positionIndex) {
            aiVector3D aiPosition =
                channel->mPositionKeys[positionIndex].mValue;
            float timeStamp = channel->mPositionKeys[positionIndex].mTime;
            KeyPosition data;
            data.position = aiVector3DToGLM(aiPosition);
            data.timeStamp = timeStamp;
            m_Positions.push_back(data);
        }

        m_NumRotations = channel->mNumRotationKeys;
        for (int rotationIndex = 0; rotationIndex < m_NumRotations;
             ++rotationIndex) {
            aiQuaternion aiOrientation =
                channel->mRotationKeys[rotationIndex].mValue;
            float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
            KeyRotation data;
            data.orientation = aiQuaternionToGLM(aiOrientation);
            data.timeStamp = timeStamp;
            m_Rotations.push_back(data);
        }

        m_NumScalings = channel->mNumScalingKeys;
        for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex) {
            aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
            float timeStamp = channel->mScalingKeys[keyIndex].mTime;
            KeyScale data;
            data.scale = aiVector3DToGLM(scale);
            data.timeStamp = timeStamp;
            m_Scales.push_back(data);
        }
    }

    void update(float animationTime) {
        glm::mat4 translation = interpolatePosition(animationTime);
        glm::mat4 rotation = interpolateRotation(animationTime);
        glm::mat4 scale = interpolateScaling(animationTime);
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

  private:
    float getScaleFactor(float lastTimeStamp, float nextTimeStamp,
                         float animationTime) const {
        float scaleFactor =
            (animationTime - lastTimeStamp) / (nextTimeStamp - lastTimeStamp);
        return scaleFactor;
    }

    glm::mat4 interpolatePosition(float animationTime) {
        if (m_NumPositions == 1)
            return glm::translate(glm::mat4(1.0f), m_Positions[0].position);

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
            auto rotation = glm::normalize(m_Rotations[0].orientation);
            return glm::toMat4(rotation);
        }

        int p0Index = getRotationIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor =
            getScaleFactor(m_Rotations[p0Index].timeStamp,
                           m_Rotations[p1Index].timeStamp, animationTime);
        glm::quat finalRotation =
            glm::slerp(m_Rotations[p0Index].orientation,
                       m_Rotations[p1Index].orientation, scaleFactor);
        finalRotation = glm::normalize(finalRotation);
        return glm::toMat4(finalRotation);
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
