#pragma once

#include <glm/glm.hpp>
#include <sstream>
#include <string>

namespace DebugGLM {

inline std::string vec3ToString(const glm::vec3 &vec) {
    std::stringstream ss;
    ss << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return ss.str();
}

inline std::string vec4ToString(const glm::vec4 &vec) {
    std::stringstream ss;
    ss << "vec4(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w
       << ")";
    return ss.str();
}

inline std::string mat4ToString(const glm::mat4 &mat) {
    std::stringstream ss;
    ss << "mat4(";
    for (int i = 0; i < 4; ++i) {
        ss << "(" << mat[i][0] << ", " << mat[i][1] << ", " << mat[i][2] << ", "
           << mat[i][3] << ")";
        if (i < 3)
            ss << ", ";
    }
    ss << ")";
    return ss.str();
}

} // namespace DebugGLM
