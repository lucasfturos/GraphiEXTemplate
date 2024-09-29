#version 330 core

in vec3 TexCoords;
out vec4 color;

uniform sampler3D uModel;

void main() {
    color = texture(uModel, TexCoords);
}
