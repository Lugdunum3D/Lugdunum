#version 450

layout (location = 0) in vec2 fragTexCoords;

layout (binding = 0) uniform sampler2D imageSampler;

layout (location = 0) out vec4 outFragColor;

void main()
{
    vec3 textColor = texture(imageSampler, fragTexCoords).rgb;
    outFragColor = vec4(textColor, 1.0);
}
