#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_VP;
uniform vec3 u_ViewPos;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec3 ViewReflectDir;
out vec3 ViewRefractDir;


void main() {
    vec4 worldPos = u_Model * vec4(aPos, 1.0);
    gl_Position = u_VP * worldPos;
    FragPos = vec3(worldPos);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    vec3 norm = normalize(aNormal);
    vec3 viewDir = FragPos - u_ViewPos;
    ViewReflectDir = reflect(viewDir, norm);
    ViewRefractDir = refract(viewDir, norm, 1.0/1.33);
}
