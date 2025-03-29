#include "shader.hpp"
#include <iostream>
#include <sstream>

Shader::Shader(const ShaderPaths &vertexPaths, const ShaderPaths &fragmentPaths)
    : m_BufferID(0) {
    std::string vertexSource = loadFilePathsShaders(vertexPaths);
    std::string fragmentSource = loadFilePathsShaders(fragmentPaths);
    m_BufferID = createShader(vertexSource, fragmentSource);
}

Shader::~Shader() { glDeleteProgram(m_BufferID); }

void Shader::bind() const { glUseProgram(m_BufferID); }

void Shader::unbind() const { glUseProgram(0); }

std::string Shader::loadFilePathsShaders(const ShaderPaths &filepaths) {
    StringSet includedFiles;
    std::stringstream combinedSource;

    for (const auto &filepath : filepaths) {
        combinedSource << loadAndProcessShader(filepath, includedFiles) << '\n';
    }

    return combinedSource.str();
}

std::string Shader::loadAndProcessShader(const std::string &filepath,
                                         StringSet &includedFiles) {
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
                std::cerr << "Malformed #include directive: " << line << '\n';
            }
        } else {
            stream << line << '\n';
        }
    }

    return stream.str();
}

GLuint Shader::createShader(const std::string &vertexSource,
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

GLuint Shader::compileShader(GLuint type, const std::string &source) {
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

GLint Shader::getUniformLocation(const std::string &name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLint location = glGetUniformLocation(m_BufferID, name.c_str());
    if (location == -1)
        std::cerr << "Warning: Uniform '" << name << "' doesn't exist!\n";

    m_UniformLocationCache[name] = location;
    return location;
}
