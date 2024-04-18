#version 410

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D tex;



void main() {
    vec4 color = texture(tex, TexCoords);
    FragColor = color.rrra;
}