#version 410 core

#define MAX_POINT_LIGHTS 20
#define MAX_SPOT_LIGHTS 20

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

struct LightBase {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
};

struct DirectionalLight {
    LightBase base;
    vec3 direction;
};

struct PointLight {
    LightBase base;
    vec3 position;
    float linear;
    float quadratic;
};

struct SpotLight {
    LightBase base;
    vec3 position;
    vec3 direction;
    float linear;
    float quadratic;
    float innerCutoff;
    float outerCutoff;
};

uniform bool directionalLightEnabled;
uniform DirectionalLight directionalLight;

uniform bool pointLightEnabled;
uniform int numPointLights;
uniform PointLight pointlights[MAX_POINT_LIGHTS];

uniform bool spotLightEnabled;
uniform int numSpotLights;
uniform SpotLight spotlights[MAX_SPOT_LIGHTS];

uniform Material material;
uniform MaterialMaps materialMaps;

uniform bool hasDiffuseMap;
uniform bool hasSpecularMap;
uniform bool hasEmissionMap;


uniform vec3 u_ViewPos;

uniform int renderMode;// 0 normal, 1 normals




vec3 calcLightColor(LightBase light, vec3 lightDir, vec3 norm) {

    vec3 diffuseColor = hasDiffuseMap ? texture(materialMaps.diffuseMap, TexCoord).rgb : material.diffuse;
    vec3 specularColor = hasSpecularMap ? texture(materialMaps.specularMap, TexCoord).rgb : material.specular;

    // ambient
    vec3 ambientColor = hasDiffuseMap ? diffuseColor : material.ambient;
    vec3 ambient = light.color * light.ambientIntensity * ambientColor;

    // diffuse
    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = light.color * light.diffuseIntensity * diff * diffuseColor;

    // specular
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    float spec = 0.0;

    // blinn vs no blinn
    if (true) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    } else {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    }
    vec3 specular =  light.color * light.specularIntensity * spec * specularColor;

    return ambient + diffuse + specular;
}

float attenuate(Light light) {
    float distToLight = length(light.position - FragPos);
    return 1.0 / (1 + light.linear * distToLight + light.quadratic * distToLight * distToLight);
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 norm) {
    vec3 lightDir = normalize(-light.direction);
    return calcLightColor(light.base, lightDir, norm);
}

vec3 calcPointLight(PointLight light, vec3 norm) {
    vec3 lightDir = normalize(light.position - FragPos);
    float attenuation = attenuate(light, distToLight);

    vec3 lightColor = calcLightColor(light.base, lightDir, norm);
    return lightColor * attenuation;
    //    return vec3(1.0,0,0);
}

vec3 calcSpotLight(SpotLight light, vec3 norm) {
    vec3 lightDir = normalize(light.position - FragPos);
    float attenuation = attenuate(light, distToLight);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    vec3 lightColor = calcLightColor(light.base, lightDir, norm);
    return lightColor * attenuation * intensity;
}

vec3 calcTotalLight() {
    vec3 totalLight = vec3(0);
    vec3 norm = normalize(Normal);
    // directional
    if (directionalLightEnabled) {
        totalLight += calcDirectionalLight(directionalLight, norm);
    }
    // point
    if (pointLightEnabled) {
        for (int i = 0; i < numPointLights; i++) {
            totalLight += calcPointLight(pointlights[i], norm);
        }
    }
    // spot
    if (spotLightEnabled) {
        for (int i = 0; i < numSpotLights; i++) {
            totalLight += calcSpotLight(spotlights[i], norm);
        }
    }
    return totalLight;
}

void main() {
    if (renderMode == 0) {
        vec3 totalLight = calcTotalLight();
        // emission
        vec3 emission = vec3(0.0);
        if (hasEmissionMap) {
            emission = texture(materialMaps.emissionMap, TexCoord).rgb;
        }
        FragColor = vec4((totalLight + emission), 1.0);
    } else if (renderMode == 1) {
        FragColor = vec4(normalize(Normal)*0.5 + 0.5, 1.0);
    } else {
        FragColor = vec4(1.0);
    }
}