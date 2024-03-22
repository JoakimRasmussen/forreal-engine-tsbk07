#version 150

out vec4 outColor;
in vec3 exNormal;
in vec2 exTexCoord;

void main(void)
{
// Anything that uses the texture coordinates!
	outColor = vec4(exTexCoord.s, exTexCoord.t, (sin(exTexCoord.s*100.0)+cos(exTexCoord.t*100.0))/2.0 + 0.5, 1.0);
}
