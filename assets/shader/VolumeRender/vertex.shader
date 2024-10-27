#version 440 core

layout(location = 0) in vec3 aPos;
out vec3 FragPos;

uniform mat4 uMVP;

void main() {
    FragPos = aPos * 0.5 + 0.5;
    gl_Position = uMVP * vec4(aPos, 1.0);
}
