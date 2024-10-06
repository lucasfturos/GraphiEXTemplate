#include "volumetric_render.hpp"
#include "Common/cube.hpp"

VolumetricRender::VolumetricRender(const std::string &filePath)
    : model(std::make_shared<Model>(filePath)),
      projMat(
          glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f)) {}

void VolumetricRender::setup() {
    loadModel();
    setupMesh();
    loadTextures();
    setUniforms();
}

void VolumetricRender::loadModel() {
    modelVertices = model->getVertices();
    modelFaces = model->getFaces();
}

void VolumetricRender::setupMesh() {
    mesh = std::make_shared<Mesh<Types>>(
        cubeVertices, cubeIndices, "assets/shader/VolumeRender/vertex.shader",
        "assets/shader/VolumeRender/fragment.shader");

    Mesh<Types>::VertexBufferLayoutMap layoutMap;

    layoutMap["vertices"] = [](std::shared_ptr<VertexBufferLayout> layout) {
        layout->push<GLfloat>(3);
    };

    mesh->setup(layoutMap);
}

std::vector<GLfloat>
VolumetricRender::generateDensityData(int width, int height, int depth) {
    std::vector<GLfloat> densityData(width * height * depth, 0.0f);
    for (size_t i = 0; i < modelFaces.size(); i += 3) {
        glm::vec3 v0 = modelVertices[modelFaces[i + 0]];
        glm::vec3 v1 = modelVertices[modelFaces[i + 1]];
        glm::vec3 v2 = modelVertices[modelFaces[i + 2]];

        for (const auto &vertex : {v0, v1, v2}) {
            float x = (vertex.x + 1.0f) / 2.0f * (width - 1);
            float y = (vertex.y + 1.0f) / 2.0f * (height - 1);
            float z = (vertex.z + 1.0f) / 2.0f * (depth - 1);

            int xi = static_cast<int>(x);
            int yi = static_cast<int>(y);
            int zi = static_cast<int>(z);

            float dx = x - xi;
            float dy = y - yi;
            float dz = z - zi;

            if (xi >= 0 && xi < width - 1 && yi >= 0 && yi < height - 1 &&
                zi >= 0 && zi < depth - 1) {
                densityData[xi + width * (yi + height * zi)] +=
                    (1 - dx) * (1 - dy) * (1 - dz);
                densityData[(xi + 1) + width * (yi + height * zi)] +=
                    dx * (1 - dy) * (1 - dz);
                densityData[xi + width * ((yi + 1) + height * zi)] +=
                    (1 - dx) * dy * (1 - dz);
                densityData[xi + width * (yi + height * (zi + 1))] +=
                    (1 - dx) * (1 - dy) * dz;
                densityData[(xi + 1) + width * ((yi + 1) + height * zi)] +=
                    dx * dy * (1 - dz);
                densityData[xi + width * ((yi + 1) + height * (zi + 1))] +=
                    (1 - dx) * dy * dz;
                densityData[(xi + 1) + width * (yi + height * (zi + 1))] +=
                    dx * (1 - dy) * dz;
                densityData[(xi + 1) +
                            width * ((yi + 1) + height * (zi + 1))] +=
                    dx * dy * dz;
            }
        }
    }
    return densityData;
}

void VolumetricRender::loadTextures() {
    int num = 32;
    int width = num;
    int height = num;
    int depth = num;

    std::vector<GLfloat> densityData =
        generateDensityData(width, height, depth);
    auto texture = std::make_shared<Texture>(width, height, depth, GL_RED,
                                             GL_FLOAT, GL_TEXTURE_3D);

    texture->updateData(densityData, width, height, depth, GL_RED, GL_FLOAT);
    mesh->setTexture(texture);
}

void VolumetricRender::setUniforms() {
    Mesh<Types>::UniformsMap uniforms;

    uniforms["uModel"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uModel", 0);
    };
    uniforms["cameraPosition"] = [this](std::shared_ptr<Shader> shader) {
        glm::vec3 cameraPos = glm::vec3(glm::inverse(viewMat)[3]);
        shader->setUniform3f("cameraPosition", cameraPos);
    };

    mesh->setUniforms(uniforms);
}

void VolumetricRender::setRunUniforms() {
    Mesh<Types>::UniformsMap uniforms;

    uniforms["uMVP"] = [this](std::shared_ptr<Shader> shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, scale * 4.0f);
        model = glm::translate(model, translation);

        glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), rotation.x,
                                                glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), -rotation.y,
                                                glm::vec3(1.0f, 0.0f, 0.0f));
        model *= rotationMatrixX * rotationMatrixY;

        glm::mat4 mvp = projMat * viewMat * model;
        shader->setUniformMat4f("uMVP", mvp);
    };

    mesh->setUniforms(uniforms);
}

void VolumetricRender::run() {
    if (!mesh)
        return;

    setRunUniforms();

    mesh->draw();
}