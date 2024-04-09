#version 150

in vec3 in_Position; // Vertex position
in vec3 in_Normal;   // Vertex normal
in vec2 in_TexCoord; // Vertex texture coordinates

out vec2 TexCoord;    // Pass texture coordinates to fragment shader
out vec3 Normal;      // Pass normal to fragment shader
out vec3 FragPos;     // Pass fragment position to fragment shader

// Updated uniforms to match your naming
uniform mat4 projectionMatrix;
uniform mat4 modelToWorld;
uniform mat4 worldToView;

void main()
{
    FragPos = vec3(modelToWorld * vec4(in_Position, 1.0)); // Transform vertex position to world space
    mat3 NormalMatrix = transpose(inverse(mat3(modelToWorld)));
    Normal = NormalMatrix * in_Normal; // Transform normal to world space

    TexCoord = in_TexCoord; // Pass texture coordinates through

    gl_Position = projectionMatrix * worldToView * modelToWorld * vec4(in_Position, 1.0);
}
