#version 150

in vec3 in_Position; // Vertex position
in vec3 in_Normal;   // Vertex normal
in vec2 in_TexCoord; // Vertex texture coordinates

out vec2 TexCoord;    // Pass texture coordinates to fragment shader
out vec3 Normal;      // Pass normal to fragment shader
out vec3 FragPos;     // Pass fragment position to fragment shader

uniform mat4 projectionMatrix;
uniform mat4 modelToWorld;
uniform mat4 worldToView;

void main()
{
    TexCoord = in_TexCoord; // Pass texture coordinates through
    
    Normal = mat3(worldToView) * mat3(modelToWorld) * in_Normal; // Transform normal to world space

    FragPos = in_Position;

    gl_Position = projectionMatrix * worldToView * modelToWorld * vec4(in_Position, 1.0);
}
