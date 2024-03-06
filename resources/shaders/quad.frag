#version 410 core

out vec4 o_Color;

in vec2 TexCoords;

uniform sampler2D tex;

void main() {
    o_Color = texture(TexCoords, tex);
}