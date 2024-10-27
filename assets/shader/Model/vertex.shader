#version 440 core

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
    vec4 pos = vec4(0.0);
    vec3 norm = vec3(0.0);

    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
        if (aBoneIDs[i] == -1) continue;
        if (aBoneIDs[i] >= MAX_BONES) {
            pos = vec4(aPos, 1.0);
            norm = aNormal;
            break;
        }
        
        vec4 localPos = uFinalBonesMatrices[aBoneIDs[i]] * vec4(aPos, 1.0);
        pos += localPos * aWeights[i];
        
        vec3 localNorm = mat3(uFinalBonesMatrices[aBoneIDs[i]]) * aNormal;
        norm += localNorm * aWeights[i];
    }

    FragPos = vec3(uModel * pos);
    gl_Position = uMVP * pos;

    mat3 normMatrix = mat3(transpose(inverse(uModel)));
    Normal = normalize(normMatrix * norm);

    TexCoords = aTexCoord;
}
