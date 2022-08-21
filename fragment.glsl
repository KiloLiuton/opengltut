#version 450 core

in vec2 UV;

out vec3 color;

uniform mat4 MVP;
uniform vec2 u_mouse;
uniform vec3 Light;

uniform sampler2D myTextureSampler;

void main()
{
    vec3 tex = texture(myTextureSampler, UV).rgb;
    color = tex;
    // light based shading goes here by altering color
    //color = color * LightColor * cosTheta
}
