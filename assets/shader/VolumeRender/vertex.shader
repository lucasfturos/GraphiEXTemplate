#version 440 core

layout(location = 0) in vec3 aPos;
out vec3 FragPos;

uniform mat4 uMVP;
uniform mat4 uModel;

void main() {
    vec3 normalizedPos = aPos * 0.5 + 0.5;
    FragPos = vec3(uModel * vec4(normalizedPos, 1.0)).xyz / vec3(4);
    gl_Position = uMVP * vec4(aPos, 1.0);
}
