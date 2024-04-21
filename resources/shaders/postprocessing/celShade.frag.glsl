#version 410

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D tex;

const float cel_levels = 5.;

void main() {
    vec4 color = texture(tex, TexCoords);

    vec4 cel_color = ceil(color * cel_levels);
    color = cel_color / cel_levels;
    float step_size = 1.0 / cel_levels;

    FragColor = color;
}
