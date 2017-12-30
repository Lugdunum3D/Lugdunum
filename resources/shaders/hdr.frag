#version 450

layout (location = 0) in vec2 fragTexCoords;

layout (binding = 0) uniform sampler2D imageSampler;

layout (location = 0) out vec4 outFragColor;

//////////////////////////////////////////////////////////////////////////////
// TONE MAPPING HELPER
//////////////////////////////////////////////////////////////////////////////

// See http://filmicworlds.com/blog/filmic-tonemapping-operators/
vec3 Uncharted2Tonemap(vec3 x) {
    const float A = 0.15f;
    const float B = 0.50f;
    const float C = 0.10f;
    const float D = 0.20f;
    const float E = 0.02f;
    const float F = 0.30f;

    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

void main()
{
    vec3 hdrColor = texture(imageSampler, fragTexCoords).rgb;
    //vec3 mappedColor = Uncharted2Tonemap(hdrColor * 4.5f) * (1.0f / Uncharted2Tonemap(vec3(11.2f)));
    outFragColor = vec4(hdrColor, 1.0);
}
