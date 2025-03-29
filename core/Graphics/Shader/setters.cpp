#include "shader.hpp"
#include <glm/gtc/type_ptr.hpp>

void Shader::setUniform1i(const std::string &name, GLint v) {
    GLint location = getUniformLocation(name);
    glUniform1i(location, v);
}

void Shader::setUniform1f(const std::string &name, GLfloat v) {
    GLint location = getUniformLocation(name);
    glUniform1f(location, v);
}

void Shader::setUniform1iv(const std::string &name,
                           const std::vector<GLuint> &v) {
    GLint location = getUniformLocation(name);
    std::vector<GLint> intValues(v.begin(), v.end());
    glUniform1iv(location, static_cast<GLsizei>(intValues.size()),
                 intValues.data());
}

void Shader::setUniform2f(const std::string &name, glm::vec2 v) {
    GLint location = getUniformLocation(name);
    glUniform2f(location, v.x, v.y);
}

void Shader::setUniform2fv(const std::string &name,
                           const std::vector<glm::vec2> &v) {
    GLint location = getUniformLocation(name);
    glUniform2fv(location, static_cast<GLsizei>(v.size()),
                 glm::value_ptr(v[0]));
}

void Shader::setUniform3f(const std::string &name, glm::vec3 v) {
    GLint location = getUniformLocation(name);
    glUniform3f(location, v.x, v.y, v.z);
}

void Shader::setUniform3fv(const std::string &name,
                           const std::vector<glm::vec3> &v) {
    GLint location = getUniformLocation(name);
    glUniform3fv(location, static_cast<GLsizei>(v.size()),
                 glm::value_ptr(v[0]));
}

void Shader::setUniform4f(const std::string &name, glm::vec4 v) {
    GLint location = getUniformLocation(name);
    glUniform4f(location, v.x, v.y, v.z, v.w);
}

void Shader::setUniform4fv(const std::string &name,
                           const std::vector<glm::vec4> &v) {
    GLint location = getUniformLocation(name);
    glUniform4fv(location, static_cast<GLsizei>(v.size()),
                 glm::value_ptr(v[0]));
}

void Shader::setUniformMat3f(const std::string &name, const glm::mat3 &m) {
    GLint location = getUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::setUniformMat3fv(const std::string &name,
                              const std::vector<glm::mat3> &m) {
    GLint location = getUniformLocation(name);
    glUniformMatrix3fv(location, static_cast<GLsizei>(m.size()), GL_FALSE,
                       glm::value_ptr(m[0]));
}

void Shader::setUniformMat4f(const std::string &name, const glm::mat4 &m) {
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::setUniformMat4fv(const std::string &name,
                              const std::vector<glm::mat4> &m) {
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, static_cast<GLsizei>(m.size()), GL_FALSE,
                       glm::value_ptr(m[0]));
}
