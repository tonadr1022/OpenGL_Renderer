#version 410

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D tex;
//uniform float contrast;
const float contrast = 0.3;

void main() {
    vec4 color = texture(tex, TexCoords);
    color.rgb = (color.rgb - 0.5) * (0.5 + contrast) + 0.5;
    FragColor = color;
}