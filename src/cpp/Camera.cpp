#include "../h/Camera.h"

Camera::Camera() : position(vec3(0.0f, 5.0f, 8.0f)), forwardVector(vec3(2.0f, 0.0f, 2.0f)), upVector(vec3(0, 1, 0)), speed(0.01f) {}

Camera::Camera(vec3 position, vec3 forwardVector, vec3 upVector, GLfloat speed) : position(position), forwardVector(forwardVector), upVector(upVector), speed(speed) {}

void Camera::update(GLfloat deltaTime) {
    // This would include logic for updating the camera's position based on input, which is currently in `keyboardInput` and `mouseMotion`.
}


// Getters
mat4 Camera::getViewMatrix() const {
    return lookAtv(position, position + forwardVector, upVector);
}
vec3 Camera::getPosition() const {
    return position;
}
vec3 Camera::getForwardVector() const {
    return normalize(forwardVector);
}
vec3 Camera::getUpVector() const {
    return upVector;
}
GLfloat Camera::getSpeed() const {
    return speed;
}
GLfloat Camera::getYaw() const {
    return yaw;
}
GLfloat Camera::getPitch() const {
    return pitch;
}

