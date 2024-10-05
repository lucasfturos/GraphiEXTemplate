#include "model_render.hpp"

ModelRender::ModelRender(const std::string &filepath)
    : model(std::make_shared<Model>(filepath)),
      animation(std::make_shared<Animation>(filepath, model)),
      animator(std::make_shared<Animator>(animation)),
      modelMat(glm::mat4(1.0f)),
      projMat(
          glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f)) {}

void ModelRender::setup() {
    loadModel();
    setupMesh();
    loadTextures();
    setUniforms();
}

void ModelRender::loadModel() {
    vertices = model->getVertices();
    faces = model->getFaces();
    normals = model->getNormals();
    boneIDs = model->getBoneIds();
    weights = model->getWeights();
}

void ModelRender::setupMesh() {
    auto texCoords = model->getTexCoords();
    mesh =
        std::make_shared<Mesh<>>(vertices, faces, normals, texCoords, boneIDs,
                                 weights, "assets/shader/Model/vertex.shader",
                                 "assets/shader/Model/fragment.shader");

    Mesh<>::VertexBufferLayoutMap layoutMap;

    layoutMap["vertices"] = [](std::shared_ptr<VertexBufferLayout> layout) {
        layout->push<GLfloat>(3);
    };
    layoutMap["normals"] = [](std::shared_ptr<VertexBufferLayout> layout) {
        layout->push<GLfloat>(3);
    };
    layoutMap["texCoords"] = [](std::shared_ptr<VertexBufferLayout> layout) {
        layout->push<GLfloat>(2);
    };
    layoutMap["boneIDs"] = [](std::shared_ptr<VertexBufferLayout> layout) {
        layout->push<GLint>(MAX_BONE_INFLUENCE);
    };
    layoutMap["weights"] = [](std::shared_ptr<VertexBufferLayout> layout) {
        layout->push<GLfloat>(MAX_BONE_INFLUENCE);
    };

    mesh->setup(layoutMap);
}

void ModelRender::loadTextures() {
    std::string diffusePath =
        "assets/model/Nightshade/textures/Nightshade_diffuse.png";
    std::string glowPath =
        "assets/model/Nightshade/textures/Nightshade_glow.png";
    std::string specularPath =
        "assets/model/Nightshade/textures/Nightshade_specular.png";
    std::string normalPath =
        "assets/model/Nightshade/textures/Nightshade_normal.png";

    auto diffuseTexture = std::make_shared<Texture>(diffusePath);
    auto glowTexture = std::make_shared<Texture>(glowPath);
    auto specularTexture = std::make_shared<Texture>(specularPath);
    auto normalTexture = std::make_shared<Texture>(normalPath);

    mesh->setTexture(diffuseTexture);
    mesh->setTexture(glowTexture);
    mesh->setTexture(specularTexture);
    mesh->setTexture(normalTexture);
}

void ModelRender::setUniforms() {
    Mesh<>::UniformsMap uniforms;

    uniforms["uTexture1"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uTexture1", 0);
    };
    uniforms["uTexture2"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uTexture2", 1);
    };
    uniforms["uTexture3"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uTexture3", 2);
    };
    uniforms["uTexture4"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uTexture4", 3);
    };
    uniforms["uLightPos"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform3f("uLightPos", glm::vec3(10.0f, 1.0f, 10.0f));
    };
    uniforms["uLightColor"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform3f("uLightColor", glm::vec3(1.0f));
    };
    uniforms["uCameraPosition"] = [this](std::shared_ptr<Shader> shader) {
        glm::vec3 cameraPos = glm::vec3(glm::inverse(viewMat)[3]);
        shader->setUniform3f("uCameraPosition", cameraPos);
    };

    mesh->setUniforms(uniforms);
}

void ModelRender::setRunUniforms() {
    Mesh<>::UniformsMap uniforms;

    uniforms["uMVP"] = [this](std::shared_ptr<Shader> shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, scale * 3.0f);
        model = glm::translate(model, translation);

        glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), rotation.x,
                                                glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), -rotation.y,
                                                glm::vec3(1.0f, 0.0f, 0.0f));
        model *= rotationMatrixX * rotationMatrixY;
        modelMat = model;

        glm::mat4 mvp = projMat * viewMat * model;
        shader->setUniformMat4f("uMVP", mvp);
    };
    uniforms["uFinalBonesMatrices"] = [this](std::shared_ptr<Shader> shader) {
        std::vector<glm::mat4> finalBoneMatrices =
            animator->getFinalBoneMatrices();
        shader->setUniformMat4f("uFinalBonesMatrices", finalBoneMatrices);
    };
    uniforms["uModel"] = [this](std::shared_ptr<Shader> shader) {
        shader->setUniformMat4f("uModel", modelMat);
    };

    mesh->setUniforms(uniforms);
}

void ModelRender::run() {
    if (!mesh || !animator)
        return;

    setRunUniforms();

    mesh->draw();
}
