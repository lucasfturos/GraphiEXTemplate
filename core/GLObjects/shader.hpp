#pragma once

#include <GL/glew.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using ShaderPaths = std::vector<std::string>;

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader {
  private:
    GLuint m_BufferID;
    std::unordered_map<std::string, GLint> m_UniformLocationCache;

  public:
    Shader(const ShaderPaths &vertexPaths, const ShaderPaths &fragmentPaths)
        : m_BufferID(0) {
        std::string vertexSource = loadFilePathsShaders(vertexPaths);
        std::string fragmentSource = loadFilePathsShaders(fragmentPaths);
        m_BufferID = createShader(vertexSource, fragmentSource);
    }

    ~Shader() { glDeleteProgram(m_BufferID); }

    void bind() const { glUseProgram(m_BufferID); }
    void unbind() const { glUseProgram(0); }

    void setUniform1i(const std::string &name, GLint value) {
        GLint location = getUniformLocation(name);
        glUniform1i(location, value);
    }

    void setUniform1f(const std::string &name, GLfloat value) {
        GLint location = getUniformLocation(name);
        glUniform1f(location, value);
    }

    void setUniform1iv(const std::string &name,
                       const std::vector<GLuint> &values) {
        GLint location = getUniformLocation(name);
        std::vector<GLint> intValues(values.begin(), values.end());
        glUniform1iv(location, static_cast<GLsizei>(intValues.size()),
                     intValues.data());
    }

    void setUniform2f(const std::string &name, glm::vec2 value) {
        GLint location = getUniformLocation(name);
        glUniform2f(location, value.x, value.y);
    }

    void setUniform3f(const std::string &name, glm::vec3 value) {
        GLint location = getUniformLocation(name);
        glUniform3f(location, value.x, value.y, value.z);
    }

    void setUniform3fv(const std::string &name,
                       const std::vector<glm::vec3> &values) {
        GLint location = getUniformLocation(name);
        glUniform3fv(location, static_cast<GLsizei>(values.size()),
                     glm::value_ptr(values[0]));
    }

    void setUniform4f(const std::string &name, glm::vec4 value) {
        GLint location = getUniformLocation(name);
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void setUniformMat3f(const std::string &name, const glm::mat3 &matrix) {
        GLint location = getUniformLocation(name);
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void setUniformMat4f(const std::string &name, const glm::mat4 &matrix) {
        GLint location = getUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void setUniformMat4fv(const std::string &name,
                          const std::vector<glm::mat4> &matrices) {
        GLint location = getUniformLocation(name);
        glUniformMatrix4fv(location, static_cast<GLsizei>(matrices.size()),
                           GL_FALSE, glm::value_ptr(matrices[0]));
    }

  private:
    std::string loadFilePathsShaders(const ShaderPaths &filepaths) {
        std::unordered_set<std::string> includedFiles;
        std::stringstream combinedSource;

        for (const auto &filepath : filepaths) {
            combinedSource << loadAndProcessShader(filepath, includedFiles)
                           << '\n';
        }

        return combinedSource.str();
    }

    std::string
    loadAndProcessShader(const std::string &filepath,
                         std::unordered_set<std::string> &includedFiles) {
        if (includedFiles.count(filepath))
            return "";

        includedFiles.insert(filepath);

        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Could not open shader file: " << filepath << '\n';
            return "";
        }

        std::stringstream stream;
        std::string line;
        std::string directory =
            filepath.substr(0, filepath.find_last_of("/\\") + 1);

        while (std::getline(file, line)) {
            if (line.find("#include") == 0) {
                std::size_t firstQuote = line.find("\"");
                std::size_t lastQuote = line.find_last_of("\"");
                if (firstQuote != std::string::npos &&
                    lastQuote != std::string::npos && lastQuote > firstQuote) {
                    std::string includePath =
                        line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                    includePath = directory + includePath;

                    stream << loadAndProcessShader(includePath, includedFiles)
                           << '\n';
                } else {
                    std::cerr << "Malformed #include directive: " << line
                              << '\n';
                }
            } else {
                stream << line << '\n';
            }
        }

        return stream.str();
    }

    GLuint createShader(const std::string &vertexSource,
                        const std::string &fragmentSource) {
        GLuint program = glCreateProgram();
        GLuint vs = compileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    GLuint compileShader(GLuint type, const std::string &source) {
        GLuint id = glCreateShader(type);
        const char *src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char *message = (char *)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            throw std::runtime_error("Failed compile shader!\n" +
                                     std::string(message) + '\n');
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    GLint getUniformLocation(const std::string &name) {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];

        GLint location = glGetUniformLocation(m_BufferID, name.c_str());
        if (location == -1)
            std::cerr << "Warning: Uniform '" << name << "' doesn't exist!\n";

        m_UniformLocationCache[name] = location;
        return location;
    }
};
