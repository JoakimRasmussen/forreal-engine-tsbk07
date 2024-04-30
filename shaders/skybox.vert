#version 150

in vec3 in_Position;
in vec2 in_TexCoord;

out vec2 TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelToWorld;
uniform mat4 worldToView;

void main()
{
    TexCoord = in_TexCoord;
    gl_Position = projectionMatrix * worldToView * modelToWorld * vec4(in_Position, 1.0);
}