#version 410 core

#define MAX_POINT_LIGHTS 20
#define MAX_SPOT_LIGHTS 20
#define far 1000
#define near 0.01

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec3 ViewReflectDir;
in vec3 ViewRefractDir;

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
uniform samplerCube skybox;

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
    //    float linear;
    //    float quadratic;
    float radius;
};

struct SpotLight {
    LightBase base;
    vec3 position;
    vec3 direction;
    float radius;
    //    float linear;
    //    float quadratic;
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

uniform int renderMode; // 0 normal, 1 normals
uniform bool useBlinn;
uniform bool reflective;

vec4 calcLightColor(LightBase light, vec3 lightDir, vec3 norm) {
    vec4 diffuseColor = hasDiffuseMap ? texture(materialMaps.diffuseMap, TexCoord) : vec4(material.diffuse, 1.0);
    if (diffuseColor.a < 1.0) {
        discard;
    }
    vec4 specularColor = hasSpecularMap ? texture(materialMaps.specularMap, TexCoord) : vec4(material.specular, 1.0);
    // vec4 specularColor = hasSpecularMap ? vec4(1.0, 0.0, 0.0, 1.0) : vec4(material.specular, 1.0);

    // ambient
    vec4 ambientColor = hasDiffuseMap ? diffuseColor : vec4(material.ambient, 1.0);
    vec4 ambient = vec4(light.color, 1.0) * light.ambientIntensity * ambientColor;

    // diffuse
    float diff = max(dot(norm, lightDir), 0);
    vec4 diffuse = vec4(light.color, 1.0) * light.diffuseIntensity * diff * diffuseColor;

    // specular
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    float spec = 0.0;
    if (useBlinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    } else {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    }
    vec4 specular = vec4(light.color, 1.0) * light.specularIntensity * spec * specularColor;

    return ambient + diffuse + specular;
}

float attenuate(vec3 lightPos, float lightRadius) {
    //    float distToLight = length(lightPos - FragPos);
    vec3 toLight = lightPos - FragPos;
    float distToLightSq = dot(toLight, toLight);
    return clamp(1.0 - distToLightSq / (lightRadius * lightRadius), 0.0, 1.0);
    //        return 1.0 / (1 + 0.09 * distToLight + 0.032 * distToLight * distToLight);
}

vec4 calcDirectionalLight(DirectionalLight light, vec3 norm) {
    vec3 lightDir = normalize(-light.direction);
    return calcLightColor(light.base, lightDir, norm);
}

vec4 calcPointLight(PointLight light, vec3 norm) {
    vec3 lightDir = normalize(light.position - FragPos);
    float attenuation = attenuate(light.position, light.radius);

    vec4 lightColor = calcLightColor(light.base, lightDir, norm);
    return lightColor * attenuation;
}

vec4 calcSpotLight(SpotLight light, vec3 norm) {
    vec3 lightDir = normalize(light.position - FragPos);
    float attenuation = attenuate(light.position, light.radius);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    vec4 lightColor = calcLightColor(light.base, lightDir, norm);
    return lightColor * attenuation * intensity;
}

vec4 calcTotalLight() {
    vec4 totalLight = vec4(0);
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

float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // 0,1 to NDC
    // inverse projection transformation
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    if (renderMode == 0) {
        vec4 totalLight = calcTotalLight();

        // emission
        vec3 emission = vec3(0.0);
        if (hasEmissionMap) {
            emission = texture(materialMaps.emissionMap, TexCoord).rgb;
        }

        vec4 color = totalLight + vec4(emission, 1.0);
        if (color.a < 0.1) {
            discard;
        }

        if (reflective) {
            vec3 viewToFrag = normalize(FragPos - u_ViewPos);
            vec4 environmentRefraction = texture(skybox, ViewRefractDir);
            vec4 environmentReflection = vec4(texture(skybox, ViewReflectDir).rgb, 1.0);
            vec4 mixedEnviron = mix(environmentReflection, environmentRefraction, 0);
            FragColor = mix(color, mixedEnviron, 1);
        } else {
            FragColor = color;
        }
    } else if (renderMode == 1) {
        FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0);
    } else if (renderMode == 2) {
        vec4 color = hasDiffuseMap ? texture(materialMaps.diffuseMap, TexCoord) : vec4(material.diffuse, 1.0);
        if (color.a < 1.0) {
            discard;
        }
        FragColor = color;
    } else if (renderMode == 3) {
        float depth = linearizeDepth(gl_FragCoord.z);
        FragColor = vec4(vec3(depth), 1.0);
    } else {
        FragColor = vec4(1.0);
    }
}
