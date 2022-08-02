#version 450 core

in vec3 fragColor;
in vec2 UV;

out vec3 color;

uniform sampler2D myTextureSampler;

void main()
{
    if (fragColor.r + fragColor.g + fragColor.b > 2.4) {
        color = texture(myTextureSampler, UV).rgb;
    } else {
        color = fragColor;
    }
}
