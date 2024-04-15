#version 150

in vec3 ex_Normal;
in vec3 surface;
in vec2 ex_TexCoord;

out vec4 out_Color;

uniform sampler2D grass, conc, dirt, map;
uniform vec3 lightPosition;
uniform vec3 objectPositions[100]; // Max number of objects
uniform int numObjects; 		   // Actual number of objects

void main(void)
{
    const vec3 lightDir = normalize(vec3(0.58, 0.58, 0.58));
    float ambientIntensity = 0.3;
    float lightIntensity = dot(normalize(ex_Normal), lightDir) + ambientIntensity;

    float shadowIntensity = 1.0;       // Start with no shadow
    float objectInfluenceRadius = 1.0; // Shadow size
    float shadowDropOff = 5.0;         // How quickly the shadow fades

    for (int i = 0; i < numObjects; i++) {
        float distance = length(surface - objectPositions[i]);
        
        if (distance < objectInfluenceRadius) {
            float shadowFactor = (objectInfluenceRadius - distance) / objectInfluenceRadius;
            shadowIntensity -= shadowFactor * 0.5; // Reduce shadow intensity based on distance
        }
    }

    shadowIntensity = max(shadowIntensity, 0.0); // No negative shadows

    vec4 m = texture(map, ex_TexCoord);
    vec4 terrainColor = (texture(grass, ex_TexCoord) * m.r +
                         texture(conc, ex_TexCoord) * m.g +
                         texture(dirt, ex_TexCoord) * m.b);

    out_Color = shadowIntensity * lightIntensity * terrainColor;
}

