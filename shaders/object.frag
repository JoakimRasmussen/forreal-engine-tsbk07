// // #version 150

// // in vec2 TexCoord; // Texture coordinates from vertex shader
// // in vec3 Normal;   // Normal from vertex shader
// // in vec3 FragPos;  // Fragment position from vertex shader

// // out vec4 color; // Output color of the pixel

// // uniform sampler2D objTex;  // The texture sampler
// // uniform vec3 lightPosition;         // Position of the light source
// // uniform vec3 viewPosition;          // Position of the camera/view

// // void main()
// // {
// //     float ambientStrength = 0.1;
// //     vec3 ambient = ambientStrength * texture(objTex, TexCoord).rgb;

// //     vec3 norm = normalize(Normal);
// //     vec3 lightDir = normalize(lightPosition - FragPos);
// //     float diff = max(dot(norm, lightDir), 0.0);
// //     vec3 diffuse = diff * texture(objTex, TexCoord).rgb;

// //     float specularStrength = 0.1;
// //     vec3 viewDir = normalize(viewPosition - FragPos);
// //     vec3 reflectDir = reflect(-lightDir, norm);  
// //     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4);
// //     vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);

// //     vec3 result = (ambient + diffuse + specular) * texture(objTex, TexCoord).rgb;
// //     color = vec4(result, 1.0);
// // }

#version 150

in vec2 TexCoord; // Texture coordinates from vertex shader
in vec3 Normal;   // Normal from vertex shader
in vec3 FragPos;  // Fragment position from vertex shader

out vec4 color; // Output color of the pixel

uniform sampler2D objTex;  // The texture sampler
uniform sampler2D debugTex;
uniform sampler2D grass;

uniform vec3 lightPosition;         // Position of the light source
uniform vec3 viewPosition;          // Position of the camera/view

void main()
{
    // Ambient Light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * texture(objTex, TexCoord).rgb;
    // vec3 ambient = ambientStrength * texture(debugTex, TexCoord).rgb;

    // Diffuse Light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture(objTex, TexCoord).rgb;
    // vec3 diffuse = diff * texture(debugTex, TexCoord).rgb;

    // Combining the results
    vec3 result = ambient + diffuse;
    color = vec4(result, 1.0);

    // color = texture(objTex, TexCoord);
    // color = texture(debugTex, TexCoord);
    // color = texture(grass, TexCoord);
    // color = vec4(TexCoord, 0.0, 1.0);

}
