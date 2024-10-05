#pragma once

#include "Common/util.hpp"
#include "texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#define MAX_BONES 100
#define MAX_BONE_INFLUENCE 4

struct BoneInfo {
    int id;
    glm::mat4 offSet;
};

class Model {
  private:
    std::vector<glm::vec3> m_Vertices;
    std::vector<glm::vec3> m_Normals;
    std::vector<glm::vec2> m_TexCoords;
    std::vector<GLuint> m_Faces;
    std::vector<GLint> m_BoneIDs;
    std::vector<GLuint> m_Weights;

    int m_BoneCounter;
    std::map<std::string, BoneInfo> m_BoneInfoMap;

    void loadModel(const std::string &filepath) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(
            filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !scene->mRootNode) {
            std::cerr << "Error loading model: " << importer.GetErrorString()
                      << '\n';
            return;
        }

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode *node, const aiScene *scene) {
        for (std::size_t i = 0; i < node->mNumMeshes; ++i) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }

        for (std::size_t i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    void processMesh(aiMesh *mesh, const aiScene * /* scene */) {
        for (std::size_t i = 0; i < mesh->mNumVertices; ++i) {
            glm::vec3 vertex = aiVector3DToGLM(mesh->mVertices[i]);
            m_Vertices.push_back(vertex);

            if (mesh->HasNormals()) {
                glm::vec3 normal = aiVector3DToGLM(mesh->mNormals[i]);
                m_Normals.push_back(normal);
            }

            glm::vec2 texCoord = mesh->mTextureCoords[0]
                                     ? glm::vec2(mesh->mTextureCoords[0][i].x,
                                                 mesh->mTextureCoords[0][i].y)
                                     : glm::vec2(0.0f, 0.0f);
            m_TexCoords.push_back(texCoord);

            m_BoneIDs.push_back(-1);
            m_Weights.push_back(0);
        }

        for (std::size_t i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (std::size_t j = 0; j < face.mNumIndices; ++j) {
                m_Faces.push_back(face.mIndices[j]);
            }
        }

        processBones(mesh);
    }

    void setVertexBoneData(int boneID, float weight) {
        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
            if (m_BoneIDs[i] < 0) {
                m_Weights[i] = weight;
                m_BoneIDs[i] = boneID;
                break;
            }
        }
    }

    void processBones(aiMesh *mesh) {
        for (std::size_t boneIndex = 0; boneIndex < mesh->mNumBones;
             ++boneIndex) {
            int boneID = -1;
            std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
            if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end()) {
                BoneInfo newBoneInfo;
                newBoneInfo.id = m_BoneCounter;
                newBoneInfo.offSet =
                    aiMatrix4x4ToGLM(mesh->mBones[boneIndex]->mOffsetMatrix);
                boneID = m_BoneCounter;
                m_BoneCounter++;
                m_BoneInfoMap[boneName] = newBoneInfo;
            } else {
                boneID = m_BoneInfoMap[boneName].id;
            }
            assert(boneID != -1);
            auto weights = mesh->mBones[boneIndex]->mWeights;
            int numWeights = mesh->mBones[boneIndex]->mNumWeights;
            for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex) {
                float weight = weights[weightIndex].mWeight;
                setVertexBoneData(boneID, weight);
            }
        }
    }

  public:
    Model(const std::string &filepath) : m_BoneCounter(0) {
        loadModel(filepath);
    }

    const auto &getFaces() const { return m_Faces; }
    const auto &getNormals() const { return m_Normals; }
    const auto &getVertices() const { return m_Vertices; }
    const auto &getTexCoords() const { return m_TexCoords; }

    const auto &getBoneIds() const { return m_BoneIDs; }
    const auto &getWeights() const { return m_Weights; }

    auto &getBoneCount() { return m_BoneCounter; }
    auto &getBoneInfoMap() { return m_BoneInfoMap; }
};
