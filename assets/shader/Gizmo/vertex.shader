#version 330 core

layout(location = 0) in vec3 aPos;
out vec3 fragPos;

uniform mat4 uMVP;

void main() {
    fragPos = aPos;
    gl_Position = uMVP * vec4(aPos, 1.0);
}