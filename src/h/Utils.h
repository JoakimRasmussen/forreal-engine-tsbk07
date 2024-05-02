#ifndef UTILS_H
#define UTILS_H

#include "GL_utilities.h"

#define M_PI 3.14159265358979323846
//#define MAIN
#define NEAR 1.0
#define FAR 1000.0
#define RIGHT 0.5
#define LEFT -0.5
#define TOP 0.5
#define BOTTOM -0.5
#define PI 3.14159265

class Utils {

public:

    Utils(){}

    GLfloat degToRad(GLfloat degrees) {
        return degrees * M_PI / 180.0f;
    }

    static const GLfloat* getProjectionMatrix() {
        static GLfloat projectionMatrix[] = 
        {
            2.0f*NEAR/(RIGHT-LEFT), 0.0f, (RIGHT+LEFT)/(RIGHT-LEFT), 0.0f,
            0.0f, 2.0f*NEAR/(TOP-BOTTOM), (TOP+BOTTOM)/(TOP-BOTTOM), 0.0f,
            0.0f, 0.0f, -(FAR+NEAR)/(FAR-NEAR), -2*FAR*NEAR/(FAR-NEAR),
            0.0f, 0.0f, -1.0f, 0.0f
        };
        return projectionMatrix;
    }

    static const int windowWidth = 2560;
    static const int windowHeight = 1440;

private:

         

};

#endif