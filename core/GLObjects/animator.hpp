#pragma once

#include "animation.hpp"

class Animator {
  public:
    Animator(std::shared_ptr<Animation> animation)
        : m_CurrentTime(0.0f), m_FinalBoneMatrices(MAX_BONES, glm::mat4(1.0)),
          m_CurrentAnimation(animation) {}

    void update(float dt) {
        if (!m_CurrentAnimation)
            return;
        m_CurrentTime += m_CurrentAnimation->getTicksPerSecond() * dt;
        m_CurrentTime =
            std::fmod(m_CurrentTime, m_CurrentAnimation->getDuration());
        calculateBoneTransform(&m_CurrentAnimation->getRootNode(),
                               glm::mat4(1.0f));
    }

    auto getFinalBoneMatrices() { return m_FinalBoneMatrices; }

  private:
    void calculateBoneTransform(const AssimpNodeData *node,
                                glm::mat4 parentTransform) {
        std::string nodeName = node->name;
        glm::mat4 nodeTransform = node->transformation;

        std::shared_ptr<Bone> bone = m_CurrentAnimation->findBone(nodeName);

        if (bone) {
            bone->update(m_CurrentTime);
            nodeTransform = bone->getLocalTransform();
        }

        glm::mat4 globalTransformation = parentTransform * nodeTransform;

        const auto &boneInfoMap = m_CurrentAnimation->getBoneIDMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
            std::size_t index = boneInfoMap.at(nodeName).id;
            glm::mat4 offset = boneInfoMap.at(nodeName).offSet;
            m_FinalBoneMatrices[index] = globalTransformation * offset;
        }

        for (int i = 0; i < node->childrenCount; ++i)
            calculateBoneTransform(node->children.data(), globalTransformation);
    }

    float m_CurrentTime;
    std::vector<glm::mat4> m_FinalBoneMatrices;
    std::shared_ptr<Animation> m_CurrentAnimation;
};
