#ifndef UTILS_H
#define UTILS_H

#include "GL_utilities.h"

#define M_PI 3.14159265358979323846

class Utils {

public:

    GLfloat degToRad(GLfloat degrees) {
        return degrees * M_PI / 180.0f;
    }

};

#endif