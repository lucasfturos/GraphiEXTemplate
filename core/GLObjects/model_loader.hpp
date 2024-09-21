#pragma once

#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

class ModelLoader {
  private:
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> faces;

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
        for (auto i = 0U; i < node->mNumMeshes; ++i) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }

        for (auto i = 0U; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    void processMesh(aiMesh *mesh, const aiScene * /*scene*/) {
        for (auto i = 0U; i < mesh->mNumVertices; ++i) {
            glm::vec3 vertex;
            vertex.x = mesh->mVertices[i].x;
            vertex.y = mesh->mVertices[i].y;
            vertex.z = mesh->mVertices[i].z;
            vertices.push_back(vertex);
        }

        for (auto i = 0U; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                faces.push_back(face.mIndices[j]);
            }
        }
    }

  public:
    ModelLoader(const std::string &filepath) { loadModel(filepath); }

    const std::vector<glm::vec3> &getVertices() const { return vertices; }

    const std::vector<GLuint> &getFaces() const { return faces; }
};

