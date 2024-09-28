#pragma once

#include "model.hpp"

struct AssimpNodeData {
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation {
  public:
    Animation() = default;
    Animation(const std::string & /* animationPath */,
              std::shared_ptr<Model> /* model */) {}
};