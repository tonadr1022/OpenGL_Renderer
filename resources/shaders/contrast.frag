#version 410

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D tex;
//uniform float contrast;
const float contrast = 0.0; // 0 contrast for now

void main() {
    vec4 color = texture(tex, TexCoords);
    color.rgb = (color.rgb - 0.5) * (1.0 + contrast) + 0.5;
    FragColor = color;
}