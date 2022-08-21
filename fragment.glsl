#version 450 core

in vec2 UV;
in vec3 Vertex_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec3 color;

//uniform mat4 MVP;
uniform vec2 u_mouse;
uniform vec3 LightPosition;
uniform vec3 LightColor;

uniform sampler2D myTextureSampler;

void main()
{
    vec3 tex = texture(myTextureSampler, UV).rgb;

    float distsq = dot(LightPosition - Vertex_worldspace, LightPosition - Vertex_worldspace);

    vec3 n = normalize(Normal_cameraspace);
    vec3 l = normalize(LightDirection_cameraspace);
    float costheta = clamp(dot(l, n), 0.0f, 1.0f);

    // light based shading
    vec3 diffuse_color = tex * LightColor * costheta / distsq;
    vec3 ambient_color = vec3(0.3f, 0.3f, 0.3f) * diffuse_color;
    color = diffuse_color + ambient_color;
}
