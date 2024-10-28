#version 440 core

in vec3 FragPos;
out vec4 color;

uniform sampler3D uVolume;
uniform sampler2D uTransferFunction;
uniform vec3 uCameraPosition;
uniform mat4 uModel;

const float stepSize = 0.01;
const int MAX_STEPS = 256;
const float EPSILON = 1.0e-5;
const float opacityFactor = 0.3;

float sdBox(vec3 p, vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sampleDensity(vec3 pos) {
    return texture(uVolume, pos).r;
}

vec3 sampleTransferFunction(float density) {
    float mappedDensity = clamp(density, 0.0, 1.0); 
    return texture(uTransferFunction, vec2(mappedDensity, 0.0)).rgb; 
}

void accumulateColorAndAlpha(inout vec4 accumulatedColor,
                             inout float accumulatedAlpha, float density) {
    if (density > EPSILON) {
        vec3 colorSample = sampleTransferFunction(density);
        float alpha = density * opacityFactor;
        accumulatedColor.rgb += (1.0 - accumulatedAlpha) * colorSample * alpha;
        accumulatedAlpha += (1.0 - accumulatedAlpha) * alpha;
    }
}

vec4 rayMarching(vec3 rayOrigin, vec3 rayDirection) {
    vec4 accumulatedColor = vec4(0.0);
    float accumulatedAlpha = 0.0;
    float t = 0.0;
    for (int i = 0; i < MAX_STEPS; ++i) {
        vec3 pos = rayOrigin + t * rayDirection;
        float d = sdBox(pos, vec3(4.0));
        if (d > 0.0) {
            break;
        }
        float density = sampleDensity(pos);
        accumulateColorAndAlpha(accumulatedColor, accumulatedAlpha, density);
        if (accumulatedAlpha >= 1.0) {
            break;
        }
        t += stepSize;
    }

    return vec4(accumulatedColor.rgb, accumulatedAlpha);
}

void main() {
    vec3 rayOrigin = FragPos;
    vec3 cameraPosModelSpace = vec3(inverse(uModel) * vec4(uCameraPosition, 1.0));
    vec3 rayDirection = normalize(rayOrigin - cameraPosModelSpace);

    color = rayMarching(rayOrigin, rayDirection);
}
