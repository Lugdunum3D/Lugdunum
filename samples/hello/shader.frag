#version 450

layout (location = 0) in vec3 verticePos;
layout (location = 1) in vec3 verticeColor;
layout (location = 2) in vec3 verticeNormal;
layout (location = 3) in vec2 verticeUv;

layout (location = 0) out vec4 color;

vec3 getAmbient(vec3 lightColor, float ambientStrength)
{
    return lightColor * ambientStrength * verticeColor;
}

vec3 getDiffuse(vec3 lightColor, vec3 fragToLight)
{
    vec3 normal = normalize(verticeNormal);
    float diff = max(dot(normal, fragToLight), 0.0);

    return diff * verticeColor * lightColor;
}

void main() {
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightPos = vec3(0.0f, -4.0f, -5.0f);
    vec3 fragToLight = normalize(lightPos - verticePos);

    vec3 ambient = getAmbient(lightColor, 0.1f);
    vec3 diffuse = getDiffuse(lightColor, fragToLight);

    color = vec4(ambient + diffuse, 1.0f);
}
