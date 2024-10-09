#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in ivec4 aBoneIDs;
layout(location = 4) in vec4 aWeights;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

#define MAX_BONES 100
#define MAX_BONE_INFLUENCE 4

uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat4 uFinalBonesMatrices[MAX_BONES];

void main() {
    mat4 boneTransform = mat4(0.0);
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
        if (aBoneIDs[i] >= 0) {
            boneTransform += uFinalBonesMatrices[aBoneIDs[i]] * aWeights[i];
        }
    }

    vec4 totalPos = boneTransform * vec4(aPos, 1.0);
    gl_Position = uMVP * totalPos;
    FragPos = vec3(uModel * totalPos);
    Normal = mat3(transpose(inverse(uModel))) * aNormal;
    TexCoords = aTexCoord;
}
