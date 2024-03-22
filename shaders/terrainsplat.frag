#version 150

// Input frag data
in vec3 ex_Normal;
in vec3 surface; // surface
in vec2 ex_TexCoord;
// Output frag data
out vec4 out_Color;

uniform sampler2D grass, conc, dirt, map;
uniform int opt;
uniform vec3 lightPosition;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	
	shade = dot(normalize(ex_Normal), light) + 0.3;

	vec4 m = texture(map, ex_TexCoord);
	out_Color = shade *
			(texture(grass, ex_TexCoord) * m.r +
			texture(conc, ex_TexCoord) * m.g +
			texture(dirt, ex_TexCoord) * m.b);
}
