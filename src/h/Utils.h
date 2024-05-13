#ifndef UTILS_H
#define UTILS_H

#include "GL_utilities.h"
#include <array>
#include <cmath>


#define M_PI 3.14159265358979323846
#define NEAR 1.0
#define FAR 1000.0
#define RIGHT 0.5
#define LEFT -0.5
#define TOP 0.5
#define BOTTOM -0.5
#define PI 3.14159265

class Utils {
public:
    Utils();  // Constructor declaration
    GLfloat degToRad(GLfloat degrees);
    static const GLfloat* getProjectionMatrix();
    static void HSVtoRGB(float h, float s, float v, float& r, float& g, float& b);
    static int colorMatch(const float color[4], float epsilon = 0.01f); // Updated to take a simple array
    static bool colorsAreEqual(const std::array<float, 3>& color1, const std::array<float, 3>& color2, float epsilon);
    static void generateColors();
    static const GLfloat* getColor(int index);
    static const bool isChanceSuccessful(int percentage);
    static float normalizeAngle(float angle);
    static float lerpAngle(float from, float to, float speed);

    // Some basic window dimensions
    // static const int windowWidth = 1920; // 2
    static const int windowWidth = 1280; // 1
    // static const int windowHeight = 1080; // 2
    static const int windowHeight = 720; // 1



    static const int numColors = 100;
private:
    static GLfloat availableColors[numColors][4];  // Static array for colors

};

#endif // UTILS_H
