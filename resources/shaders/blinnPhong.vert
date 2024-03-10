#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 u_Model;
//uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_VP;
//uniform mat3 u_NormalMatrix;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;


void main() {
    gl_Position = u_VP * u_Model * vec4(aPos, 1.0);
//    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
    FragPos = vec3(u_Model * vec4(aPos, 1.0f));
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
}
