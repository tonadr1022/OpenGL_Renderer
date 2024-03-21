#version 410 core

out vec4 o_Color;

in vec2 TexCoord;

uniform sampler2D tex;


void main() {
    o_Color = texture(tex, TexCoord);
//    o_Color = vec4(texColor.r * u_Color.r, texColor.g * u_Color.g, texColor.b * u_Color.b, texColor.a);
}