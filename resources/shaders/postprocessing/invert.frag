#version 410

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
    vec4 color = texture(tex, TexCoords);
    FragColor = vec4(vec3(1.0 - color), 1.0);
}