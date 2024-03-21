#version 410

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D tex;

uniform float u_Contrast;

void main() {
    vec4 color = texture(tex, TexCoords);
    color.rgb = (color.rgb - 0.5) * (1.0 + u_Contrast) + 0.5;
    FragColor = color;
}