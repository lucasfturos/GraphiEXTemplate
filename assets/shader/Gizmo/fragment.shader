#version 440 core

in vec3 fragPos;
out vec4 color;

void main() {
    vec3 posAbs = abs(fragPos);
    vec4 fragColor = vec4(0.0);
    if (posAbs.x > posAbs.y && posAbs.x > posAbs.z) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red
    } else if (posAbs.y > posAbs.x && posAbs.y > posAbs.z) {
        fragColor = vec4(0.0, 1.0, 0.0, 1.0); // Green
    } else if (posAbs.z > posAbs.x && posAbs.z > posAbs.y) {
        fragColor = vec4(0.0, 0.0, 1.0, 1.0); // Blue
    } else {
        fragColor = vec4(1.0, 1.0, 0.0, 1.0); // Yellow
    }
    color = fragColor;
}
