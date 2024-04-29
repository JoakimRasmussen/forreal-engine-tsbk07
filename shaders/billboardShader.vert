#version 150

in  vec3 in_Position;
//in  vec3 inColor;
//in  vec3 inNormal;
in vec2 in_TexCoord;

//out vec3 exColor; // Gouraud
//out vec3 exNormal; // Phong
out vec2 texCoord;

uniform mat4 modelToViewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	texCoord = in_TexCoord;
	
	gl_Position = projectionMatrix * modelToViewMatrix * vec4(in_Position, 1.0);
}