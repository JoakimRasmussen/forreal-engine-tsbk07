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
    Normal = mat3(transpose(inverse(modelToWorld))) * in_Normal; // Adjust normals for lighting calculations

    TexCoord = in_TexCoord; // Pass texture coordinates through

    // Final transformation to clip space
    gl_Position = projectionMatrix * worldToView * vec4(FragPos, 1.0);
}
