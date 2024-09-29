#pragma once

#include "Common/util.hpp"
#include "bone.hpp"
#include "model.hpp"

#include <algorithm>

struct AssimpNodeData {
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation {
  public:
    Animation() = default;
    Animation(const std::string &animationPath, std::shared_ptr<Model> model)
        : m_Duration(0.0f), m_TicksPerSecond(0), m_RootNode(), m_Bones(),
          m_BoneInfoMap() {
        Assimp::Importer importer;

        const aiScene *scene =
            importer.ReadFile(animationPath, aiProcess_Triangulate);
        assert(scene && scene->mRootNode);
        if (scene->mNumAnimations == 0)
            throw std::runtime_error(
                "Erro: Nenhuma animação encontrada no arquivo.");

        auto animation = scene->mAnimations[0];
        m_Duration = animation->mDuration;
        m_TicksPerSecond = animation->mTicksPerSecond;

        readHeirarchyData(m_RootNode, scene->mRootNode);
        readMissingBones(animation, model);
    }

    std::shared_ptr<Bone> findBone(const std::string &name) {
        auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
                                 [&](const std::shared_ptr<Bone> &bone) {
                                     return bone->getBoneName() == name;
                                 });

        return (iter != m_Bones.end()) ? *iter : nullptr;
    }

    inline float getDuration() { return m_Duration; }
    inline float getTicksPerSecond() { return m_TicksPerSecond; }
    inline const AssimpNodeData &getRootNode() { return m_RootNode; }
    inline const std::map<std::string, BoneInfo> &getBoneIDMap() {
        return m_BoneInfoMap;
    }

  private:
    void readMissingBones(const aiAnimation *animation,
                          std::shared_ptr<Model> &model) {
        int size = animation->mNumChannels;

        auto &boneInfoMap = model->getBoneInfoMap();
        int boneCount = model->getBoneCount();

        for (int i = 0; i < size; ++i) {
            auto channel = animation->mChannels[i];
            std::string boneName = channel->mNodeName.data;
            if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
                boneInfoMap[boneName].id = boneCount;
                boneCount++;
            }
            m_Bones.emplace_back(
                std::make_shared<Bone>(boneInfoMap[channel->mNodeName.data].id,
                                       channel->mNodeName.data, channel));
        }

        m_BoneInfoMap = boneInfoMap;
    }

    void readHeirarchyData(AssimpNodeData &dest, const aiNode *src) {
        assert(src);

        dest.name = src->mName.data;
        dest.transformation = aiMatrix4x4ToGLM(src->mTransformation);
        dest.childrenCount = src->mNumChildren;

        for (std::size_t i = 0; i < src->mNumChildren; ++i) {
            AssimpNodeData newData;
            readHeirarchyData(newData, src->mChildren[i]);
            dest.children.push_back(newData);
        }
    }

    float m_Duration;
    int m_TicksPerSecond;
    AssimpNodeData m_RootNode;
    std::vector<std::shared_ptr<Bone>> m_Bones;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
};