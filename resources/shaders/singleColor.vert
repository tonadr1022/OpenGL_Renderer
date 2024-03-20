#version 410

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;

uniform mat4 u_VP;
uniform mat4 u_Model;

void main() {
    TexCoords = aTexCoords;
    Normal = aNormal;
    gl_Position = u_VP * u_Model * vec4(aPos + normalize(aNormal) * 0.08, 1.0);
}