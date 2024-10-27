#version 440 core

out vec4 color;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;

uniform vec3 uLightPos;
uniform vec3 uCameraPosition;
uniform vec3 uLightColor;

void main() {
    vec4 diffuseColor = texture(uTexture1, TexCoords);
    vec4 glowColor = texture(uTexture2, TexCoords);
    vec4 specularColor = texture(uTexture3, TexCoords);

    vec3 norm = Normal;
    vec3 lightDir = normalize(uLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(uCameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 ambient = 0.2 * vec3(diffuseColor);
    vec3 diffuse = diff * vec3(diffuseColor) * uLightColor;
    vec3 specular = spec * vec3(specularColor) * uLightColor;

    vec3 finalColor = ambient + diffuse + specular;
    color = vec4(finalColor, 1.0) + glowColor * 0.5;
}
