#ifndef CAMERA_H
#define CAMERA_H

#include "VectorUtils4.h"
#include "GL_utilities.h"
#include "InputController.h"

class Camera {
public:

    // Camera variables
    vec3 position;
    vec3 forwardVector;
    vec3 upVector;
    GLfloat speed = 0.01f;
    GLfloat yaw = -90.0f;
    GLfloat pitch = 0.0f;

    // Constructor
    Camera();
    Camera(vec3 position, vec3 forwardVector, vec3 upVector, GLfloat speed);
    void update(GLfloat deltaTime);
    mat4 getViewMatrix() const;

    // Getters
    vec3 getPosition() const;
    vec3 getForwardVector() const;
    vec3 getUpVector() const;
    GLfloat getSpeed() const;
    GLfloat getYaw() const;
    GLfloat getPitch() const;
};

#endif
