#version 440 core

out vec4 color;

uniform float uTime;

void main() {
    float r = sin(uTime) * 0.5 + 0.5;
    float g = sin(uTime + 2.0) * 0.5 + 0.5;
    float b = sin(uTime + 4.0) * 0.5 + 0.5;

    color = vec4(r, g, b, 1.0);
}
