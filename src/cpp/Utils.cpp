#include "../h/Utils.h"


GLfloat Utils::availableColors[Utils::numColors][4];

Utils::Utils() {
    // Constructor implementation if needed
}

GLfloat Utils::degToRad(GLfloat degrees) {
    return degrees * M_PI / 180.0f;
}

const GLfloat* Utils::getProjectionMatrix() {
    static GLfloat projectionMatrix[] = {
        2.0f*NEAR/(RIGHT-LEFT), 0.0f, (RIGHT+LEFT)/(RIGHT-LEFT), 0.0f,
        0.0f, 2.0f*NEAR/(TOP-BOTTOM), (TOP+BOTTOM)/(TOP-BOTTOM), 0.0f,
        0.0f, 0.0f, -(FAR+NEAR)/(FAR-NEAR), -2*FAR*NEAR/(FAR-NEAR),
        0.0f, 0.0f, -1.0f, 0.0f
    };
    return projectionMatrix;
}

void Utils::HSVtoRGB(float h, float s, float v, float& r, float& g, float& b) {
    int i = int(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }
}

int Utils::colorMatch(const float color[4], float epsilon) {
    for (int i = 0; i < numColors; i++) {
        if (colorsAreEqual({color[0], color[1], color[2]}, {availableColors[i][0], availableColors[i][1], availableColors[i][2]}, epsilon)) {
            return i;  // Return the index of the matching color
        }
    }
    return -1;  // Return -1 if no match is found
}

bool Utils::colorsAreEqual(const std::array<float, 3>& color1, const std::array<float, 3>& color2, float epsilon) {
    return (std::fabs(color1[0] - color2[0]) < epsilon) && // Use std::fabs instead of fabs
           (std::fabs(color1[1] - color2[1]) < epsilon) &&
           (std::fabs(color1[2] - color2[2]) < epsilon);
}

const GLfloat* Utils::getColor(int index) {
    if (index >= 0 && index < numColors) {
        return availableColors[index];
    }
    static GLfloat defaultColor[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // Return white or any other default color
    return defaultColor; // Safety: return a default color if index is out of range
}


void Utils::generateColors() {
    const float saturation = 1.0f;  // Full saturation
    const float value = 1.0f;       // Full brightness
    for (int i = 0; i < numColors; i++) {
        float hue = (float)i / numColors;  // Distribute hues evenly
        GLfloat r, g, b;
        HSVtoRGB(hue, saturation, value, r, g, b);  // Convert to RGB
        availableColors[i][0] = r;
        availableColors[i][1] = g;
        availableColors[i][2] = b;
        availableColors[i][3] = 1.0f;  // Alpha channel
    }
}
