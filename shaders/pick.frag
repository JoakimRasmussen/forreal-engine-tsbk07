#version 150

uniform vec4 objectColor; // Unique color for each object, passed as a uniform
out vec4 outColor;        // Output color of the fragment

void main() {
    outColor = objectColor; // Set the output color to the uniform color
}
