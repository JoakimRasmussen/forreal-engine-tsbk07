#version 150

// Input frag data
in vec3 ex_Normal;
in vec3 surf;
in vec2 ex_TexCoord;
// Output frag data
out vec4 out_Color;

uniform sampler2D tex;
uniform vec3 lightPosition;

void main(void)
{
	float diffuse, specular, shade;
	vec3 lightPosition = vec3(100, 50, 100);
	vec3 light = vec3(0.75, 0.75, 0.75); // can be switched out for lightDirection
	light = normalize(light);
	vec3 lightDirection = normalize(lightPosition - surf);
	
	// Diffuse
	diffuse = dot(normalize(ex_Normal), lightDirection);
	diffuse = max(0.001, diffuse); // No negative lightDirection
	
	// Specular
	vec3 r = reflect(-lightDirection, normalize(ex_Normal));
	vec3 v = normalize(-surf); // View direction
	specular = dot(r, v);
	if (specular > 0.0)
		specular = 1.0 * pow(specular, 150.0);

	specular = max(specular, 0.001);
	shade = 0.7*diffuse + 1.0*specular;

	// Texture
	vec4 texColor = texture(tex, ex_TexCoord);

	out_Color = texColor * vec4(shade, shade, shade, 1.0);
}
