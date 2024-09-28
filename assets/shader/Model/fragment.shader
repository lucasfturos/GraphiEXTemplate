#version 330 core

out vec4 color;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

void main() {
    vec4 diffuseColor = texture(texture1, TexCoords);
    vec4 glowColor = texture(texture2, TexCoords);
    vec4 specularColor = texture(texture3, TexCoords);

    vec4 finalColor =
        diffuseColor * 0.8 + glowColor * 0.5 + specularColor * 0.2;
    color = finalColor;
}
