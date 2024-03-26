#version 150

// Input frag data
in vec3 ex_Normal;
in vec3 surface; // surface
in vec2 ex_TexCoord;
// Output frag data
out vec4 out_Color;

// Uniforms
uniform sampler2D maskrosTexture, rockTexture;
uniform vec3 lightPosition;
uniform float mountainHeight;

void main(void)
{
	float diffuse, specular, shade;
	vec3 lightPosition = vec3(100, 50, 100);
	vec3 light = vec3(0.75, 0.75, 0.75); // can be switched out for lightDirection
	light = normalize(light);
	vec3 lightDirection = normalize(lightPosition - surface);
	
	// Diffuse
	diffuse = dot(normalize(ex_Normal), lightDirection);
	diffuse = max(0.001, diffuse); // No negative light
	
	// Specular
	vec3 r = reflect(-lightDirection, normalize(ex_Normal));
	vec3 v = normalize(-surface); // View direction
	specular = dot(r, v);
	if (specular > 0.0)
		specular = 1.0 * pow(specular, 150.0);

	specular = max(specular, 0.001);
	shade = 0.7*diffuse + 1.0*specular;

	// Texture
	vec4 maskrosColor = texture(maskrosTexture, ex_TexCoord);
	vec4 rockColor = texture(rockTexture, ex_TexCoord);

	if (surface.y < mountainHeight)	
    {
        float t = clamp((mountainHeight - surface.y) / mountainHeight, 0.0, 1.0); // Calculate the interpolation factor based on surface.y
        out_Color = mix(rockColor, maskrosColor, t) * vec4(shade, shade, shade, 1.0);
    }
    else
    {
        // Use maskrosColor directly if surface.y >= 15
        out_Color = rockColor * vec4(shade, shade, shade, 1.0);
    }
}
