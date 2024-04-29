// #version 150

// in vec3 ex_Normal;
// in vec3 surface;
// in vec2 ex_TexCoord;

// out vec4 out_Color;

// uniform sampler2D grass, rock, dirt, map;
// uniform vec3 lightPosition;
// uniform vec3 objectPositions[100]; // Max number of objects
// uniform int numObjects; 		   // Actual number of objects

// void main(void)
// {
//     const vec3 lightDir = normalize(vec3(0.58, 0.58, 0.58));
//     float ambientIntensity = 0.3;
//     float lightIntensity = dot(normalize(ex_Normal), lightDir) + ambientIntensity;

//     float shadowIntensity = 1.0;       // Start with no shadow
//     float objectInfluenceRadius = 1.0; // Shadow size
//     float shadowDropOff = 10.0;         // How quickly the shadow fades

//     for (int i = 0; i < numObjects; i++) {
//         float distance = length(surface - objectPositions[i]);
        
//         if (distance < objectInfluenceRadius) {
//             float shadowFactor = (objectInfluenceRadius - distance) / objectInfluenceRadius;
//             shadowIntensity -= shadowFactor * 0.5; // Reduce shadow intensity based on distance
//         }
//     }

//     shadowIntensity = max(shadowIntensity, 0.0); // No negative shadows

//      // Calculate angle between the normal and the vertical (upward) direction
//     float angle = degrees(acos(dot(normalize(ex_Normal), vec3(0.0, 1.0, 0.0))));

//     // Texture coordinates multiplied by a factor for repetition
//     vec2 texCoords = ex_TexCoord * 25.0;

//     vec4 m = texture(map, ex_TexCoord);
//     vec4 terrainColor;

//     // Choose texture based on the slope angle
//     if (angle > 60.0) {
//         terrainColor = texture(rock, texCoords); // Use dirt texture on steep slopes
//     } else {
//         // Use mix of textures based on 'map' texture weights
//         terrainColor = (texture(grass, texCoords) * m.r +
//                         texture(dirt, texCoords) * m.g +
//                         texture(rock, texCoords) * m.b);
//     }

//     out_Color = shadowIntensity * lightIntensity * terrainColor;
// }

#version 150

in vec3 ex_Normal;
in vec3 surface;
in vec2 ex_TexCoord;

out vec4 out_Color;

uniform sampler2D grass, rock, dirt, map;
uniform vec3 lightPosition;
uniform vec3 objectPositions[100]; // Max number of objects
uniform int numObjects;             // Actual number of objects

void main(void) {
    const vec3 lightDir = normalize(vec3(0.58, 0.58, 0.58));
    float ambientIntensity = 0.1; // Reduced ambient light
    float lightIntensity = dot(normalize(ex_Normal), lightDir) + ambientIntensity;

    float shadowIntensity = 1.0;       // Start with no shadow
    float objectInfluenceRadius = 1.0; // Shadow size
    float shadowDropOff = 10.0;        // How quickly the shadow fades

    for (int i = 0; i < numObjects; i++) {
        float distance = length(surface - objectPositions[i]);
        if (distance < objectInfluenceRadius) {
            float shadowFactor = (objectInfluenceRadius - distance) / objectInfluenceRadius;
            shadowIntensity -= shadowFactor * 0.8 * exp(-distance / shadowDropOff);
        }
    }

    shadowIntensity = max(shadowIntensity, 0.1); // Ensure minimum shadow darkness

    // Calculate angle between the normal and the vertical (upward) direction
    float angle = degrees(acos(dot(normalize(ex_Normal), vec3(0.0, 1.0, 0.0))));

    // Texture coordinates multiplied by a factor for repetition
    vec2 texCoords = ex_TexCoord * 25.0;

    vec4 m = texture(map, ex_TexCoord);
    vec4 terrainColor;

    // Choose texture based on the slope angle
    if (angle > 60.0) {
        terrainColor = texture(rock, texCoords); // Use rock texture on steep slopes
    } else {
        // Use mix of textures based on 'map' texture weights
        terrainColor = (texture(grass, texCoords) * m.r +
                        texture(dirt, texCoords) * m.g +
                        texture(rock, texCoords) * m.b);
    }

    out_Color = vec4(1.0) * terrainColor; // Base color
    out_Color *= shadowIntensity; // Apply shadow intensity
    out_Color *= lightIntensity; // Apply light intensity
}
