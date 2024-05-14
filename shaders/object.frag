#version 150

in vec2 TexCoord; // Texture coordinates from vertex shader
in vec3 Normal;   // Normal from vertex shader
in vec3 FragPos;  // Fragment position from vertex shader

out vec4 color; // Output color of the pixel

uniform sampler2D objectTexture;    // The texture sampler
uniform vec3 lightPosition;         // Position of the light source
uniform vec3 viewPosition;          // Position of the camera/view

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * texture(objectTexture, TexCoord).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture(objectTexture, TexCoord).rgb;

    float specularStrength = 0.1;
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4);
    vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);

    vec3 result = (ambient + diffuse + specular) * texture(objectTexture, TexCoord).rgb;
    color = vec4(result, 1.0);
}
