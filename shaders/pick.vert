#version 150

in vec3 in_Position;

uniform mat4 projectionMatrix;
uniform mat4 modelToWorld;
uniform mat4 worldToView;

void main() {
    // Transform the vertex position from model space to clip space
    gl_Position = projectionMatrix * worldToView * modelToWorld * vec4(in_Position, 1.0);
}
