#version 450

layout (location = 0) in vec2 fragTexCoords;

layout (binding = 0) uniform sampler2D imageSampler;

layout (constant_id = 0) const int blurdirection = 0;

layout (location = 0) out vec4 outFragColor;

// Gaussian blur weights for a 21x21 kernel: http://dev.theomader.com/gaussian-kernel-calculator/
const float weight[11] = float[] (0.035822, 0.05879, 0.086425, 0.113806, 0.13424, 0.141836, 0.13424, 0.113806, 0.086425, 0.05879, 0.035822);

void main()
{
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    vec2 pixelSize = 1.0f / textureSize(imageSampler, 0);

    // Horizontal
    if (blurdirection == 1)
    {
        for (int i = -5; i <= 5; ++i)
        {
            result += texture(imageSampler, fragTexCoords + vec2(pixelSize.x * i, 0.0)).rgb * weight[i + 5];
        }
        outFragColor = vec4(result, 1.0);
    }
    // Vertical
    else
    {
        for (int i = -5; i <= 5; ++i)
        {
            result += texture(imageSampler, fragTexCoords + vec2(0.0, pixelSize.y * i)).rgb * weight[i + 5];
        }
        outFragColor = vec4(result, 1.0);
    }
}
