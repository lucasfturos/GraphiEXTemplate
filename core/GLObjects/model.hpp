#pragma once

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
    glm::mat4 boneOffset;
};

class Model {
  private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<GLuint> faces;
    std::vector<GLuint> boneIds;
    std::vector<GLuint> weights;

    int numBones;
    std::vector<BoneInfo> boneInfo;
    std::map<std::string, int> boneMapping;
    std::vector<std::vector<std::pair<int, float>>> vertexBoneWeights;

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

        vertexBoneWeights.resize(scene->mMeshes[0]->mNumVertices);

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode *node, const aiScene *scene) {
        for (auto i = 0U; i < node->mNumMeshes; ++i) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }

        for (auto i = 0U; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    void processMesh(aiMesh *mesh, const aiScene * /* scene */) {
        for (auto i = 0U; i < mesh->mNumVertices; ++i) {
            glm::vec3 vertex;
            vertex.x = mesh->mVertices[i].x;
            vertex.y = mesh->mVertices[i].y;
            vertex.z = mesh->mVertices[i].z;
            vertices.push_back(vertex);

            if (mesh->HasNormals()) {
                glm::vec3 normal;
                normal.x = mesh->mNormals[i].x;
                normal.y = mesh->mNormals[i].y;
                normal.z = mesh->mNormals[i].z;
                normals.push_back(normal);
            }

            if (mesh->mTextureCoords[0]) {
                glm::vec2 texCoord;
                texCoord.x = mesh->mTextureCoords[0][i].x;
                texCoord.y = mesh->mTextureCoords[0][i].y;
                texCoords.push_back(texCoord);
            } else {
                texCoords.push_back(glm::vec2(0.0f, 0.0f));
            }

            boneIds.push_back(-1);
            weights.push_back(0);
        }

        for (auto i = 0U; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (auto j = 0U; j < face.mNumIndices; j++) {
                faces.push_back(face.mIndices[j]);
            }
        }

        processBones(mesh);
    }

    void processBones(aiMesh *mesh) {
        for (auto i = 0U; i < mesh->mNumBones; ++i) {
            int boneIndex = 0;
            std::string boneName(mesh->mBones[i]->mName.data);
            if (boneMapping.find(boneName) == boneMapping.end()) {
                boneIndex = numBones;
                numBones++;
                BoneInfo boneInfo;
                boneInfo.id = boneIndex;
                boneInfo.boneOffset = glm::mat4(1.0f);
                this->boneInfo.push_back(boneInfo);
                boneMapping[boneName] = boneIndex;
            } else {
                boneIndex = boneMapping[boneName];
            }

            for (auto j = 0U; j < mesh->mBones[i]->mNumWeights; ++j) {
                unsigned int vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
                float weight = mesh->mBones[i]->mWeights[j].mWeight;

                for (int k = 0; k < MAX_BONE_INFLUENCE; ++k) {
                    if (vertexBoneWeights[vertexID].size() <
                        MAX_BONE_INFLUENCE) {
                        vertexBoneWeights[vertexID].emplace_back(boneIndex,
                                                                 weight);
                        break;
                    }
                }

                boneIds[vertexID] = boneIndex;
                weights[vertexID] += weight;
            }

            const aiMatrix4x4 &offsetMatrix = mesh->mBones[i]->mOffsetMatrix;
            this->boneInfo[boneIndex].boneOffset =
                glm::mat4(offsetMatrix.a1, offsetMatrix.b1, offsetMatrix.c1,
                          offsetMatrix.d1, offsetMatrix.a2, offsetMatrix.b2,
                          offsetMatrix.c2, offsetMatrix.d2, offsetMatrix.a3,
                          offsetMatrix.b3, offsetMatrix.c3, offsetMatrix.d3,
                          offsetMatrix.a4, offsetMatrix.b4, offsetMatrix.c4,
                          offsetMatrix.d4);
        }
    }

  public:
    Model(const std::string &filepath) : numBones(0) { loadModel(filepath); }

    const std::vector<glm::vec3> &getVertices() const { return vertices; }
    const std::vector<glm::vec3> &getNormals() const { return normals; }
    const std::vector<glm::vec2> &getTexCoords() const { return texCoords; }
    const std::vector<GLuint> &getFaces() const { return faces; }
    const std::vector<GLuint> &getBoneIds() const { return boneIds; }
    const std::vector<GLuint> &getWeights() const { return weights; }

    const std::vector<std::vector<std::pair<int, float>>> &
    getBoneWeights() const {
        return vertexBoneWeights;
    }
    const std::map<std::string, int> &getBoneMapping() const {
        return boneMapping;
    }
    const std::vector<BoneInfo> &getBoneInfo() const { return boneInfo; }
};
