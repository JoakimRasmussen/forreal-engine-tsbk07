#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTexCoord;
out vec3 exNormal;
out vec2 exTexCoord;

uniform mat4 modelviewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	exNormal = inverse(transpose(mat3(modelviewMatrix))) * inNormal; // Phong, "fake" normal transformation

	exTexCoord = inTexCoord;

	gl_Position = projectionMatrix * modelviewMatrix * vec4(inPosition, 1.0); // This should include projection
}
