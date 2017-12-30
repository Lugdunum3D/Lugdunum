#version 450

#define PI 3.1415926535897932384626433832795

layout (set = 1, binding = 0) uniform sampler2D skyBox;

layout (location = 0) in vec3 inPos;

layout (location = 0) out vec4 outSceneColor;
layout (location = 1) out vec4 outGlowColor;

const float BLUR_THRESHOLD = 0.7;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    vec3 color = texture(skyBox, SampleSphericalMap(normalize(inPos))).rgb;
    outSceneColor = vec4(color, 1.0);

    float brightness = (color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722);
    if (brightness > BLUR_THRESHOLD) {
        outGlowColor = vec4(color, 1.0);
    } else {
        outGlowColor = vec4(0.0);
    }
}
