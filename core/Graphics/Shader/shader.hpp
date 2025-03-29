#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using ShaderPaths = std::vector<std::string>;

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader {
  public:
    Shader(const ShaderPaths &, const ShaderPaths &);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform1i(const std::string &, GLint);
    void setUniform1f(const std::string &, GLfloat);
    void setUniform1iv(const std::string &, const std::vector<GLuint> &);

    void setUniform2f(const std::string &, glm::vec2);
    void setUniform2fv(const std::string &, const std::vector<glm::vec2> &);

    void setUniform3f(const std::string &, glm::vec3);
    void setUniform3fv(const std::string &, const std::vector<glm::vec3> &);

    void setUniform4f(const std::string &, glm::vec4);
    void setUniform4fv(const std::string &, const std::vector<glm::vec4> &);

    void setUniformMat3f(const std::string &, const glm::mat3 &);
    void setUniformMat3fv(const std::string &, const std::vector<glm::mat3> &);

    void setUniformMat4f(const std::string &, const glm::mat4 &);
    void setUniformMat4fv(const std::string &, const std::vector<glm::mat4> &);

  private:
    using StringSet = std::unordered_set<std::string>;
    using UniformLocationMap = std::unordered_map<std::string, GLint>;

    GLuint m_BufferID;
    UniformLocationMap m_UniformLocationCache;

    std::string loadFilePathsShaders(const ShaderPaths &);
    std::string loadAndProcessShader(const std::string &, StringSet &);

    GLuint createShader(const std::string &, const std::string &);
    GLuint compileShader(GLuint, const std::string &);

    GLint getUniformLocation(const std::string &);
};
