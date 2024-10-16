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

uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat4 uFinalBonesMatrices[MAX_BONES];

void main() {
    mat4 boneTransform = mat4(0.0);
    boneTransform += uFinalBonesMatrices[int(aBoneIDs.x)] * aWeights.x;
    boneTransform += uFinalBonesMatrices[int(aBoneIDs.y)] * aWeights.y;
    boneTransform += uFinalBonesMatrices[int(aBoneIDs.z)] * aWeights.z;
    boneTransform += uFinalBonesMatrices[int(aBoneIDs.w)] * aWeights.w;

    vec4 pos = boneTransform * vec4(aPos, 1.0);
    gl_Position = uMVP * pos;
    FragPos = vec3(uModel * pos);

    mat3 normalMatrix = mat3(transpose(inverse(uModel)));
    vec3 norm = mat3(boneTransform) * aNormal;
    Normal = normalMatrix * norm;

    TexCoords = aTexCoord;
}
