#version 440 core

layout(location = 0) in vec3 aPos;
out vec3 TexCoords;
out vec3 VRayDir;
flat out vec3 TransformedEye;

uniform mat4 uMVP;
uniform vec3 uVolumeScale;
uniform vec3 uCameraPosition;

void main() {
    vec3 volumeTranslation = vec3(0.5) - uVolumeScale * 0.5;
    TexCoords = aPos * 0.5 + 0.5;
    gl_Position = uMVP * vec4(aPos * uVolumeScale + volumeTranslation, 1);

    TransformedEye = (uCameraPosition - volumeTranslation) / uVolumeScale;
    VRayDir = aPos - TransformedEye;
}
