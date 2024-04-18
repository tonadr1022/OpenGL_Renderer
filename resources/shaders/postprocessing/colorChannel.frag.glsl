#version 410

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D tex;

uniform vec4 u_ColorChannels; // rgba

void main() {
    vec4 color = texture(tex, TexCoords);
    FragColor = vec4(color.r * u_ColorChannels.r, color.g * u_ColorChannels.g, color.b * u_ColorChannels.b, color.a * u_ColorChannels.a);
}