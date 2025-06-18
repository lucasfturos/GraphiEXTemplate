#version 440 core

in vec3 FragPos;
out vec4 color;

uniform sampler3D uVolume;
uniform sampler2D uTransferFunction;

uniform mat4 uModel;
uniform vec3 uTranslation;
uniform vec3 uCameraPosition;

const float STEP_SIZE = 5e-3;
const int MAX_STEPS = 256;
const float EPS = 1.0e-5;
const float OPACITY_FACTOR = 0.1;

float sdBox(vec3 p, vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sampleDensity(vec3 pos) {
    return texture(uVolume, clamp(pos * 0.5 + 0.5, 0.0, 1.0)).r;
}

vec3 sampleTransferFunction(float density) {
    float mappedDensity = clamp(density, 0.0, 1.0);
    return texture(uTransferFunction, vec2(mappedDensity, 0.0)).rgb;
}

vec3 computeNormal(vec3 pos) {
    float delta = 0.01;
    float dx = sampleDensity(pos + vec3(delta, 0.0, 0.0)) - sampleDensity(pos - vec3(delta, 0.0, 0.0));
    float dy = sampleDensity(pos + vec3(0.0, delta, 0.0)) - sampleDensity(pos - vec3(0.0, delta, 0.0));
    float dz = sampleDensity(pos + vec3(0.0, 0.0, delta)) - sampleDensity(pos - vec3(0.0, 0.0, delta));
    return normalize(vec3(dx, dy, dz));
}

void accumulateColorAndAlpha(inout vec4 accumulatedColor,
                             inout float accumulatedAlpha,
                             float density, vec3 pos) {
    if (density > EPS) {
        vec3 colorSample = sampleTransferFunction(density);

        vec3 normal = computeNormal(pos);
        vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
        float diffuse = max(dot(normal, lightDir), 0.0);
        colorSample *= diffuse * 0.8 + 0.2;

        float alpha = density * OPACITY_FACTOR;
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
        pos -= uTranslation;

        if (any(lessThan(pos, vec3(-1.01))) || any(greaterThan(pos, vec3(1.01)))) {
            break;
        }

        float density = sampleDensity(pos);
        accumulateColorAndAlpha(accumulatedColor, accumulatedAlpha, density, pos);
        if (accumulatedAlpha >= 1.0) {
            break;
        }

        float step = STEP_SIZE;
        if (density < 0.05) {
            step *= 2.0;
        }
        t += step;
    }

    return vec4(accumulatedColor.rgb, accumulatedAlpha);
}

void main() {
    vec3 rayOrigin = vec3(inverse(uModel) * vec4(FragPos, 1.0));
    vec3 newCameraPos = vec3(inverse(uModel) * vec4(uCameraPosition, 1.0));
    vec3 rayDirection = normalize(rayOrigin - newCameraPos);

    color = rayMarching(rayOrigin, rayDirection);
}
