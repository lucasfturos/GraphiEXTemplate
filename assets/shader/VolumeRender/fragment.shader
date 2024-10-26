#version 440 core

in vec3 TexCoords;
out vec4 color;

uniform sampler3D uModel;
uniform vec3 cameraPosition;

const float stepSize = 0.01;
const int MAX_STEPS = 256;
const float EPSILON = 1.0e-5;
const float opacityFactor = 0.5;

float sdBox(vec3 p, vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float opSmoothIntersection(float d1, float d2, float k) {
    float h = clamp(0.5 - 0.5 * (d2 - d1) / k, 0.0, 1.0);
    return mix(d2, d1, h) + k * h * (1.0 - h);
}

float sampleDensity(vec3 pos) { return texture(uModel, pos).r; }

void accumulateColorAndAlpha(inout vec4 accumulatedColor,
                             inout float accumulatedAlpha, float density) {
    if (density > EPSILON) {
        vec3 colorSample = vec3(density);
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
        float d = sdBox(pos, vec3(2.0));
        if (d > 0.0) {
            break;
        }

        float density = sampleDensity(pos);
        accumulateColorAndAlpha(accumulatedColor, accumulatedAlpha, density);
        if (accumulatedAlpha >= 1.0) {
            break;
        }
        t += stepSize * d;
    }

    return vec4(accumulatedColor.rgb, accumulatedAlpha);
}

void main() {
    vec3 rayOrigin = TexCoords;
    vec3 rayDirection = normalize(TexCoords - cameraPosition);
    color = rayMarching(rayOrigin, rayDirection);
}
