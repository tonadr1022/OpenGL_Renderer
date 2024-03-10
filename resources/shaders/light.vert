#version 410 core

layout (location = 0) in vec3 aPos;

uniform mat4 u_VP;
uniform mat4 u_Model;

void main() {
    gl_Position = u_VP * u_Model * vec4(aPos, 1);
}