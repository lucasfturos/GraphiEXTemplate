#version 440 core

layout(location = 0) in vec3 aPos;
out vec3 FragPos;

uniform mat4 uMVP;
uniform mat4 uModel;

void main() {
    FragPos = (uModel * vec4(aPos, 1.0)).xyz;
    gl_Position = uMVP * vec4(aPos, 1.0);
}
