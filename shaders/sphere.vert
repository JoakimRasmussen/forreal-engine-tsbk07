#version 150

// Input for vertex shader
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

// Uniforms for vertex shader
uniform mat4 projectionMatrix;
uniform mat4 modelToWorld;
uniform mat4 worldToView;

// Output of vertex shader
out vec3 ex_Normal;
out vec3 surf;
out vec2 ex_TexCoord;

void main(void)
{
	ex_TexCoord = in_TexCoord;
	ex_Normal = mat3(worldToView) * mat3(modelToWorld) * in_Normal;
	surf = in_Position;
	gl_Position = projectionMatrix * worldToView * modelToWorld * vec4(in_Position, 1.0);
}
