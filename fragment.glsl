#version 450 core

in vec3 fragColor;
in vec2 UV;

out vec3 color;

uniform sampler2D myTextureSampler;

void main()
{
    vec3 tex = texture(myTextureSampler, UV);
    if (tex.r + tex.g + tex.b > 2.4) {
        color = fragColor;
    } else {
        color = tex;
    }
	color = fragColor;
}
