#version 150

in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D skyboxTex;

void main(){
    outColor = texture(skyboxTex, TexCoord);
}