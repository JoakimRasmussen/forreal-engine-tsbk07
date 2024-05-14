#include "../h/Utils.h"
#include <stdexcept>

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

/**
 * Converts a color from HSV (Hue, Saturation, Value) color space to RGB (Red, Green, Blue) color space.
 *
 * @param h The hue value (between 0 and 1).
 * @param s The saturation value (between 0 and 1).
 * @param v The value (brightness) value (between 0 and 1).
 * @param r Reference to the variable where the resulting red component will be stored.
 * @param g Reference to the variable where the resulting green component will be stored.
 * @param b Reference to the variable where the resulting blue component will be stored.
 */
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

/**
 * Finds the index of the color that matches the given color within a specified epsilon.
 *
 * @param color The color to match, represented as an array of four floats (RGBA).
 * @param epsilon The maximum difference allowed between the colors for them to be considered a match.
 * @return The index of the matching color, or -1 if no match is found.
 */
int Utils::colorMatch(const float color[4], float epsilon) {
    for (int i = 0; i < numColors; i++) {
        if (colorsAreEqual({color[0], color[1], color[2]}, {availableColors[i][0], availableColors[i][1], availableColors[i][2]}, epsilon)) {
            return i;  // Return the index of the matching color
        }
    }
    return -1;  // Return -1 if no match is found
}

/**
 * Checks if two colors are approximately equal.
 *
 * @param color1 The first color represented as an array of three floats (RGB values).
 * @param color2 The second color represented as an array of three floats (RGB values).
 * @param epsilon The maximum difference allowed between color components for them to be considered equal.
 * @return True if the colors are approximately equal, false otherwise.
 */
bool Utils::colorsAreEqual(const std::array<float, 3>& color1, const std::array<float, 3>& color2, float epsilon) {
    return (std::fabs(color1[0] - color2[0]) < epsilon) && // Use std::fabs instead of fabs
           (std::fabs(color1[1] - color2[1]) < epsilon) &&
           (std::fabs(color1[2] - color2[2]) < epsilon);
}

/**
 * Returns the color at the specified index.
 * 
 * @param index The index of the color to retrieve.
 * @return A pointer to the color value.
 *         If the index is within the valid range, the color at the specified index is returned.
 *         If the index is out of range, a default color (white) is returned.
 */
const GLfloat* Utils::getColor(int index) {
    if (index >= 0 && index < numColors) {
        return availableColors[index];
    }
    static GLfloat defaultColor[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // Return white or any other default color
    return defaultColor; // Safety: return a default color if index is out of range
}


/**
 * Generates an array of colors using the HSV color model.
 * Each color is represented as an RGBA value.
 */
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

/**
 * Determines whether the game object should perform an action based on a given percentage.
 * 
 * @param percentage The percentage chance (1 to 100) of the game object performing an action.
 * @return True if the game object should perform the action, false otherwise.
 */

bool const Utils::isChanceSuccessful(int percentage) {
    if (percentage < 1 || percentage > 100) {
        throw std::invalid_argument("Percentage must be between 1 and 100.");
    }
    return (rand() % 100) < percentage;
}

/**
 * Normalizes an angle to the range [-PI, PI].
 *
 * @param angle The angle to be normalized.
 * @return The normalized angle.
 */
float Utils::normalizeAngle(float angle) {
    // Normalize angle to range [-PI, PI]
    angle = fmod(angle + M_PI, 2 * M_PI);
    if (angle < 0) angle += 2 * M_PI;
    angle -= M_PI;

    // Snap values close to -PI or PI to -PI or PI to handle precision issues at boundaries
    const float epsilon = 1e-5;  // A small threshold to handle precision
    if (fabs(angle - M_PI) < epsilon) {
        angle = M_PI;
    } else if (fabs(angle + M_PI) < epsilon) {
        angle = -M_PI;
    }
    return angle;
}

/**
 * Linearly interpolates between two angles, taking the shortest path.
 * 
 * @param from The starting angle.
 * @param to The target angle.
 * @param speed The speed of interpolation.
 * @return The interpolated angle.
 */
float Utils::lerpAngle(float from, float to, float speed) {
    float difference = to - from;
    if (difference > M_PI) {
        difference -= 2 * M_PI;  // Adjust for wrap-around
    } else if (difference < -M_PI) {
        difference += 2 * M_PI;  // Adjust for wrap-around
    }

    float adjusted = from + difference * speed;
    return normalizeAngle(adjusted);  // Normalize the result
}

