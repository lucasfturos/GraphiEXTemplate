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

vec4 Pos = vec4(aPos, 1.0);

void main() {
    vec4 newPos = vec4(0.0f);
    vec3 newNorm = vec3(0.0f);

    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
        if (aBoneIDs[i] == -1) continue;
        if (aBoneIDs[i] >= MAX_BONES) {
            newPos = Pos;
            newNorm = aNormal;
            break;
        }
        
        vec4 localPos = uFinalBonesMatrices[aBoneIDs[i]] * Pos;
        newPos += localPos * aWeights[i];

        vec3 localNorm = mat3(uFinalBonesMatrices[aBoneIDs[i]]) * aNormal;
        newNorm += localNorm * aWeights[i];
    }

    gl_Position = uMVP * newPos;
    FragPos = vec3(uModel * newPos);
    Normal = mat3(transpose(inverse(uModel))) * newNorm;
    TexCoords = aTexCoord;
}
