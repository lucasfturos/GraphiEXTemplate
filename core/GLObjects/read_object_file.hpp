#pragma once

#include "Common/util.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class ReadObjectFile {
  private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> faces;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textures;

    void reset();
    void parseLine(const std::string &line);
    void parseFace(const std::string &line, std::istringstream &iss);
    void loadFile(const std::string &filePath);

  public:
    ReadObjectFile(const std::string &filePath);

    MeshDataObj getMeshData() const;
};