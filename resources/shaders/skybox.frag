#version 410

in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube tex;

void main() {
    FragColor = texture(tex, TexCoords);
}