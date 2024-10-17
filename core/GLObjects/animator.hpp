#pragma once

#include "animation.hpp"

class Animator {
  public:
    Animator()
        : m_CurrentTime(0.0f), m_Interpolating(false), m_HaltTime(0.0f),
          m_InterTime(0.0f), m_FinalBoneMatrices(MAX_BONES, glm::mat4(1.0)),
          m_CurrentAnimation(nullptr), m_NextAnimation(nullptr),
          m_QueueAnimation(nullptr) {}

    void play(std::shared_ptr<Animation> animation) {
        if (!m_CurrentAnimation) {
            m_CurrentAnimation = animation;
            return;
        }

        if (m_Interpolating) {
            if (animation != m_NextAnimation)
                m_QueueAnimation = animation;
        } else {
            if (animation != m_NextAnimation) {
                m_Interpolating = true;
                m_HaltTime =
                    std::fmod(m_CurrentTime, m_CurrentAnimation->getDuration());
                m_NextAnimation = animation;
                m_CurrentTime = 0.0f;
                m_InterTime = 0.0;
            }
        }
    }

    void update(float dt) {
        if (m_CurrentAnimation) {
            m_CurrentTime = std::fmod(
                m_CurrentTime + m_CurrentAnimation->getTicksPerSecond() * dt,
                m_CurrentAnimation->getDuration());

            float transitionTime =
                m_CurrentAnimation->getTicksPerSecond() * 0.2f;
            if (m_Interpolating && m_InterTime <= transitionTime) {
                m_InterTime += m_CurrentAnimation->getTicksPerSecond() * dt;
                calculateBoneTransition(&m_CurrentAnimation->getRootNode(),
                                        glm::mat4(1.0f), m_CurrentAnimation,
                                        m_NextAnimation, m_HaltTime,
                                        m_InterTime, transitionTime);
                return;
            } else if (m_Interpolating) {
                if (m_QueueAnimation) {
                    m_CurrentAnimation = m_NextAnimation;
                    m_HaltTime = 0.0f;
                    m_NextAnimation = m_QueueAnimation;
                    m_QueueAnimation = nullptr;
                    m_CurrentTime = 0.0f;
                    m_InterTime = 0.0f;
                    return;
                }
                m_Interpolating = false;
                m_CurrentAnimation = m_NextAnimation;
                m_CurrentTime = 0.0f;
                m_InterTime = 0.0f;
            }

            calculateBoneTransform(&m_CurrentAnimation->getRootNode(),
                                   glm::mat4(1.0f));
        }
    }

    auto getFinalBoneMatrices() { return m_FinalBoneMatrices; }

  private:
    void calculateBoneTransform(const AssimpNodeData *node,
                                glm::mat4 parentTransform) {
        auto nodeName = node->name;
        auto nodeTransform = node->transformation;

        auto bone = m_CurrentAnimation->findBone(nodeName);
        if (bone) {
            bone->update(m_CurrentTime);
            nodeTransform = bone->getLocalTransform();
        }

        auto finalTransform = parentTransform * nodeTransform;

        const auto &boneInfoMap = m_CurrentAnimation->getBoneInfoMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
            auto index = boneInfoMap.at(nodeName).id;
            auto offset = boneInfoMap.at(nodeName).offSet;
            m_FinalBoneMatrices[index] = finalTransform * offset;
        }

        for (int i = 0; i < node->childrenCount; ++i)
            calculateBoneTransform(&node->children[i], finalTransform);
    }

    void calculateBoneTransition(const AssimpNodeData *curNode,
                                 glm::mat4 parentTransform,
                                 std::shared_ptr<Animation> prevAnimation,
                                 std::shared_ptr<Animation> nextAnimation,
                                 float haltTime, float currentTime,
                                 float transitionTime) {
        std::string nodeName = curNode->name;
        glm::mat4 transform = curNode->transformation;

        auto prevBone = prevAnimation->findBone(nodeName);
        auto nextBone = nextAnimation->findBone(nodeName);

        if (prevBone && nextBone) {
            KeyPosition prevPos = prevBone->getPositions(haltTime);
            KeyRotation prevRot = prevBone->getRotations(haltTime);
            KeyScale prevScl = prevBone->getScalings(haltTime);

            KeyPosition nextPos = nextBone->getPositions(0.0f);
            KeyRotation nextRot = nextBone->getRotations(0.0f);
            KeyScale nextScl = nextBone->getScalings(0.0f);

            glm::mat4 p = interpolatePosition(currentTime, prevPos, nextPos);
            glm::mat4 r = interpolateRotation(currentTime, prevRot, nextRot);
            glm::mat4 s = interpolateScaling(currentTime, prevScl, nextScl);

            transform = p * r * s;
        }

        glm::mat4 globalTransformation = parentTransform * transform;
        auto boneInfoMap = nextAnimation->getBoneInfoMap();

        if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
            auto index = boneInfoMap.at(nodeName).id;
            auto offset = boneInfoMap.at(nodeName).offSet;
            m_FinalBoneMatrices[index] = globalTransformation * offset;
        }

        for (int i = 0; i < curNode->childrenCount; ++i)
            calculateBoneTransition(&curNode->children[i], globalTransformation,
                                    prevAnimation, nextAnimation, haltTime,
                                    currentTime, transitionTime);
    }

    float m_CurrentTime;
    bool m_Interpolating;
    float m_HaltTime;
    float m_InterTime;
    std::vector<glm::mat4> m_FinalBoneMatrices;
    std::shared_ptr<Animation> m_CurrentAnimation;
    std::shared_ptr<Animation> m_NextAnimation;
    std::shared_ptr<Animation> m_QueueAnimation;
};
