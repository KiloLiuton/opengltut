#version 450 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

// Output vertex information is interpolated for each fragment
out vec2 UV;
out vec3 Vertex_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform vec3 LightPosition;


void main()
{
    vec4 v4 = vec4(vertexPosition_modelspace, 1);
    gl_Position = MVP * v4;

    Vertex_worldspace = (M * v4).xyz;

    vec3 vertex_position_cameraspace = (V * M * v4).xyz;
    EyeDirection_cameraspace = -vertex_position_cameraspace;

    // Model matrix for a point light is always Identity
    vec3 light_position_cameraspace = (V * vec4(LightPosition, 1)).xyz;
    LightDirection_cameraspace = light_position_cameraspace - vertex_position_cameraspace;

    // Only correct if there is no scalling on the Model (M) matrix. But why?
    Normal_cameraspace = (V * M * vec4(vertexNormal, 0)).xyz;

    UV = vertexUV;
}
