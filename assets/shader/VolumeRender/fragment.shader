#version 440 core

in vec3 TexCoords;
in vec3 VRayDir;
flat in vec3 TransformedEye;

out vec4 color;


uniform float uDtScale;
uniform sampler3D uModel;
uniform ivec3 uVolumeDimension;
uniform sampler2D uTransferFunction;

const float EPSILON = 1.0e-5;

vec2 intersectBox(vec3 orig, vec3 dir) {
    const vec3 boxMin = vec3(0.0);
    const vec3 boxMax = vec3(1.0);
    vec3 invDir = 1.0 / dir;
    vec3 tMinTmp = (boxMin - orig) * invDir;
    vec3 tMaxTmp = (boxMax - orig) * invDir;
    vec3 tMin = min(tMinTmp, tMaxTmp);
    vec3 tMax = max(tMinTmp, tMaxTmp);
    float t0 = max(tMin.x, max(tMin.y, tMin.z));
    float t1 = min(tMax.x, min(tMax.y, tMax.z));
    return vec2(t0, t1);
}

void main(void) {
    vec3 rayDir = normalize(VRayDir);

    vec2 tHit = intersectBox(TransformedEye, rayDir);
    if (tHit.x > tHit.y) {
        discard;
    }

    vec3 dtVec = 1.0 / (vec3(uVolumeDimension) * abs(rayDir));
    float dt = min(dtVec.x, min(dtVec.y, dtVec.z)) * uDtScale;

    vec3 p = TransformedEye + tHit.x * rayDir;
	for (float t = tHit.x; t < tHit.y; t += dt) {
		float val = texture(uModel, p).r;
		vec4 valColor = vec4(texture(uTransferFunction, vec2(val, 0.5)).rgb, val);

		color.rgb += (1.0 - color.a) * valColor.a * valColor.rgb;
		color.a += (1.0 - color.a) * valColor.a;

		if (color.a >= 0.9) {
			break;
		}
		p += rayDir * dt;
	}
}