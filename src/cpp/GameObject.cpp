#include "../h/GameObject.h"

// Constructor definition
GameObject::GameObject(Model* model, float x, float y, float z, float rx, float ry, float rz)
    : model(model), x(x), y(y), z(z), rx(rx), ry(ry), rz(rz) {
}

void GameObject::move(float dx, float dy, float dz) {
    x += dx;
    y += dy;
    z += dz;
}

void GameObject::newDestination() {
    // move to a random destination within 10 blocks of the current position
    targetX = x + (rand() % 20 - 10);
    targetZ = z + (rand() % 20 - 10);
    destinationReached = false;
}

void GameObject::moveTowardsDestination() {
    if (destinationReached) {
        newDestination();
    }

    float dx = targetX - x;
    float dz = targetZ - z;
    float distance = sqrt(dx * dx + dz * dz);

    if (distance < 0.1f) {
        destinationReached = true;
        return;
    }

    // Update rotation
    float desiredRy = atan2(dx, dz);
    ry = lerpAngle(ry, desiredRy, 0.05f);

    // Move forward if aligned with the destination
    if (fabs(ry - desiredRy) < 0.2) {  // Increased tolerance to 0.2 radians
        float speed = 0.05f;
        x += speed * dx / distance;
        z += speed * dz / distance;
        // printf("Moving: x = %f, z = %f, ry = %f, desiredRy = %f\n", x, z, ry, desiredRy);
    }
    // else {
    //     printf("Not moving, rotation not aligned: ry = %f, desiredRy = %f\n", ry, desiredRy);
    // }

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

float GameObject::lerpAngle(float from, float to, float t) {
    float difference = to - from;
    if (difference > M_PI) {
        difference -= 2 * M_PI;  // Adjust for wrap-around
    } else if (difference < -M_PI) {
        difference += 2 * M_PI;  // Adjust for wrap-around
    }

    float adjusted = from + difference * t;
    
    // Normalize the result to keep within -PI to PI
    if (adjusted > M_PI) {
        adjusted -= 2 * M_PI;
    } else if (adjusted < -M_PI) {
        adjusted += 2 * M_PI;
    }
    return adjusted;
}


