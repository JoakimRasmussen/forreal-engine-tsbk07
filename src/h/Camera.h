#ifndef CAMERA_H
#define CAMERA_H

#define NEAR 1.0
#define FAR 1000.0
#define RIGHT 0.5
#define LEFT -0.5
#define TOP 0.5
#define BOTTOM -0.5

#include "VectorUtils4.h"
#include "GL_utilities.h"
#include "InputController.h"

class Camera {
public:

    // Camera variables
    vec3 position;
    vec3 forwardVector;
    vec3 upVector;
    GLfloat speed = 0.1f;
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

    // TODO: fix this implementation, it is implemented in run.cpp as GLfloat projectionMatrix[]
    mat4 getProjectionMatrix()
    {
        mat4 projectionMatrix ={
            2.0f*NEAR/(RIGHT-LEFT), 0.0f, (RIGHT+LEFT)/(RIGHT-LEFT), 0.0f,
            0.0f, 2.0f*NEAR/(TOP-BOTTOM), (TOP+BOTTOM)/(TOP-BOTTOM), 0.0f,
            0.0f, 0.0f, -(FAR+NEAR)/(FAR-NEAR), -2*FAR*NEAR/(FAR-NEAR),
            0.0f, 0.0f, -1.0f, 0.0f
        };
        return projectionMatrix;
    }

private:


};

#endif
