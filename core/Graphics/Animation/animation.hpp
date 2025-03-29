#pragma once

#include "Graphics/model.hpp"
#include "bone.hpp"

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
        : m_Duration(0.0f), m_TicksPerSecond(0) {
        Assimp::Importer importer;

        const aiScene *scene =
            importer.ReadFile(animationPath, aiProcess_Triangulate);
        assert(scene && scene->mRootNode);
        if (scene->mNumAnimations == 0)
            std::cerr << "Error: No animation found in file.\n";

        auto animation = scene->mAnimations[0];
        m_Duration = animation->mDuration;
        m_TicksPerSecond = animation->mTicksPerSecond;

        readHeirarchyData(m_RootNode, scene->mRootNode);
        m_RootNode.transformation = glm::mat4(1.0f);

        readMissingBones(animation, model);
    }

    std::shared_ptr<Bone> findBone(const std::string &name) {
        auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
                                 [&](const std::shared_ptr<Bone> &bone) {
                                     return bone->getBoneName() == name;
                                 });
        return (iter == m_Bones.end()) ? nullptr : *iter;
    }

    inline float getDuration() { return m_Duration; }
    inline float getTicksPerSecond() { return m_TicksPerSecond; }

    inline const auto *getRootNode() { return &m_RootNode; }
    inline const auto &getBoneInfoMap() { return m_BoneInfoMap; }

  private:
    void readMissingBones(const aiAnimation *animation,
                          std::shared_ptr<Model> &model) {
        auto &boneInfoMap = model->getBoneInfoMap();
        int &boneCount = model->getBoneCount();

        for (auto i = 0U; i < animation->mNumChannels; ++i) {
            auto channel = animation->mChannels[i];
            auto boneName = channel->mNodeName.data;
            if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
                boneInfoMap[boneName].id = boneCount;
                ++boneCount;
            }
            m_Bones.push_back(
                std::make_shared<Bone>(boneInfoMap[channel->mNodeName.data].id,
                                       channel->mNodeName.data, channel));
        }
        m_BoneInfoMap = boneInfoMap;
    }

    void readHeirarchyData(AssimpNodeData &dest, const aiNode *src) {
        assert(src);

        dest.name = src->mName.data;
        dest.transformation =
            AssimpGLMConversion::aiMatrix4x4ToGLM(src->mTransformation);
        dest.childrenCount = src->mNumChildren;

        for (auto i = 0U; i < src->mNumChildren; ++i) {
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