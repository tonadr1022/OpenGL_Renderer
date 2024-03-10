#version 410 core

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct MaterialMaps {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D emissionMap;
    sampler2D normalMap;
};


struct Light {
    int type; // 0 for directional, 1 for point, 2 for spot
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light pointLight;
uniform Light directionalLight;

uniform Material material;
uniform MaterialMaps materialMaps;

uniform bool hasDiffuseMap;
uniform bool hasSpecularMap;
uniform bool hasEmissionMap;

uniform vec3 u_ViewPos;

vec3 getLightColor(Light light) {
    vec3 lightDir = vec3(0);
    if (light.type == LIGHT_TYPE_DIRECTIONAL) {
        lightDir = normalize(-directionalLight.direction);
    } else if (light.type == LIGHT_TYPE_POINT) {
        lightDir = normalize(pointLight.position - FragPos);
    }

    vec3 norm = normalize(Normal);

    vec3 diffuseColor = hasDiffuseMap ? texture(materialMaps.diffuseMap, TexCoord).rgb : material.diffuse;
    vec3 specularColor = hasSpecularMap ? texture(materialMaps.specularMap, TexCoord).rgb : material.specular;

    // ambient
    vec3 ambientColor = hasDiffuseMap ? diffuseColor : material.ambient;
    vec3 ambient = light.ambient * ambientColor;

    // diffuse
    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * diffuseColor;

    // specular
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular =  light.specular * (spec * specularColor);

    return ambient + diffuse + specular;
}






void main() {

    vec3 pointResult = getLightColor(pointLight);
    vec3 directionalResult = getLightColor(directionalLight);

    vec3 result = pointResult + directionalResult;

    // emission
    vec3 emission = vec3(0.0);
    if (hasEmissionMap) {
        emission = texture(materialMaps.emissionMap, TexCoord).rgb;
    }

    FragColor = vec4((result + emission), 1.0);
}