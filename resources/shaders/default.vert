#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 u_Model;
uniform mat4 u_VP;

out vec2 TexCoord;


void main() {
    gl_Position = u_VP * u_Model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
