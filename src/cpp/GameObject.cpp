#include "../h/GameObject.h"


void GameObject::move(float dx, float dy, float dz) {
    x += dx;
    y += dy;
    z += dz;
}

void GameObject::newDestination() {
    // move to a random destination within 10 blocks of the current position
    targetX = x + (rand() % 20 - 10);
    targetZ = z + (rand() % 20 - 10);
    // printf("Current position: %f, %f\n", x, z);
    // printf("New destination: %f, %f\n", targetX, targetZ);
    destinationReached = false;
}

void GameObject::moveTowardsDestination() {
    if (destinationReached)
    {
        newDestination();
        return;
    }

    float dx = targetX - x;
    float dz = targetZ - z;
    float distance = sqrt(dx*dx + dz*dz);

    if (distance < 0.1f) {
        destinationReached = true;
        return;
    }

    // calculate the new rotation to face the destination
    ry = atan2(dx, dz);

    // calculate the new position
    float speed = 0.05f;
    x += speed * dx / distance;
    z += speed * dz / distance;
}

void GameObject::setPosition(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

vec3 GameObject::getPosition() const {
    float x = this->x;
    float y = this->y;
    float z = this->z;
    return vec3(x, y, z);
}

void GameObject::setRotation(float rx, float ry, float rz) {
    this->rx = rx;
    this->ry = ry;
    this->rz = rz;
}

vec3 GameObject::getRotation() const {
    float rx = this->rx;
    float ry = this->ry;
    float rz = this->rz;
    return vec3(rx, ry, rz);
}

void GameObject::updateAlignment(vec3 normal, vec3 toCamera, bool forceRotation) {
    rx = atan2(normal.z, normal.y);
    if (forceRotation) ry = atan2(toCamera.x, toCamera.z);
    rz = 0.0f;
}