#include "../h/GameObject.h"

// Constructor definition
GameObject::GameObject(Model* model, int objectID, float x, float y, float z, float rx, float ry, float rz, bool sleeping)
    : model(model), x(x), y(y), z(z), rx(rx), ry(ry), rz(rz), sleeping(sleeping) {
    this->objectID = objectID;
}

// Destructor definition
GameObject::~GameObject() {
    // Destructor
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
    if (sleeping) return;
    if (destinationReached) {
        newDestination();
    }

    float dx = targetX - x;
    float dz = targetZ - z;
    float distance = sqrt(dx * dx + dz * dz);

    // Handle jumping and gravity
    if (isJumping) {
        y += yVelocity; // Apply vertical velocity
        jumpTime++;
        yVelocity -= 0.01f + 0.0025f * jumpTime; // Gravity effect, adjust as needed

        // Check if landed
        if (y <= 0) {
            y = 0;
            yVelocity = 0;
            isJumping = false;
            jumpTime = 0;
        }
    }

    if (distance < 0.1f) {
        destinationReached = true;
        return;
    }

    // Horizontal movement towards destination
    // if (!isJumping || y == 0) { // Move horizontally if not jumping or has landed
        float desiredRy = atan2(dx, dz);
        desiredRy = normalizeAngle(desiredRy);
        ry = lerpAngle(ry, desiredRy, turnSpeed);
        ry = normalizeAngle(ry);

        if (fabs(ry - desiredRy) < 0.2 || turnIterations > max_iterations) {
            x += speed * dx / distance;
            z += speed * dz / distance;
            turnIterations = 0;
            // jump(); // Jump after reaching destination
        } else {
            turnIterations++;
        }
    // }
}

void GameObject::jump() {
    // printf("Trying to jump\n");
    if (!isJumping) {
        // printf("Jumping\n");
        yVelocity = 0.5f; // Initial jump velocity, adjust as needed
        isJumping = true;
    }
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

void GameObject::updateAlignmentToTerrain(vec3 normal) {
    rx = atan2(normal.z, normal.y);
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

float GameObject::normalizeAngle(float angle) {
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


