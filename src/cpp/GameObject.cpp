#include "../h/GameObject.h"

// -------------------------------
// Constructors and Destructors
// -------------------------------

GameObject::GameObject(Model* model, int objectID, float x, float y, float z, float rx, float ry, float rz, bool sleeping)
    : model(model), x(x), y(y), z(z), rx(rx), ry(ry), rz(rz), sleeping(sleeping) {
    this->objectID = objectID;
    newDestination();
}

GameObject::~GameObject() {
    // Destructor
}
// -------------------------------
// Position and Movement Handling
// -------------------------------

void GameObject::move(float dx, float dy, float dz) {
    x += dx;
    y += dy;
    z += dz;
}

void GameObject::moveTowardsDestination(float distance) {
    handleHorizontalMovement(distance);
}

/**
 * Moves the game object.
 * If the object is jumping, it handles the jumping behavior.
 * Otherwise, it selects a new random direction and rotates smoothly towards it.
 * If the object should move forward in the new direction, it starts a jump.
 */
void GameObject::bunnyMovement() {

    // Check if the object is jumping
    if (isJumping) {
        handleJumping();
        return;
    }
    // Select new random direction
    if (prepareNewJump) {
        if (!shouldJump(1)) {
        return;
        }
        jumpDirection = randomDirection();
        prepareNewJump = false;
    }

    // Rotate smoothly towards the new direction
    float desiredRotation = calculateDesiredRotation(jumpDirection.x, jumpDirection.y);
    rotateTowards(desiredRotation);

    // Jump forward in the new direction
    if (shouldMoveForward(desiredRotation)) {
        startJump(2.0f);
    }
}

void GameObject::newDestination() {
    // move to a random destination within 10 blocks of the current position
    targetX = x + (rand() % 20 - 10);
    targetZ = z + (rand() % 20 - 10);
}

void GameObject::logic() {
    if (sleeping) return;
    bunnyMovement(); // New movement logic for bunny
    // float distanceLeft = distanceToTarget();
    // if (isCloseEnough(distanceLeft)) {
    //     // Destination reached. Set new destination
    //     newDestination();
    // }
    // else {
    //     moveTowardsDestination(distanceLeft);
    // }
}

float GameObject::distanceToTarget() {
    dx = targetX - x;
    dz = targetZ - z;
    return sqrt(dx * dx + dz * dz);
}

bool GameObject::isCloseEnough(float distance) {
    return distance < 0.1f;
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

// -------------------------------
// Rotation and Orientation
// -------------------------------

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

float GameObject::calculateDesiredRotation(float dx, float dz) {
    float angle = atan2(dx, dz);
    return normalizeAngle(angle);
}

void GameObject::rotateTowards(float desiredRy) {
    ry = lerpAngle(ry, desiredRy, turnSpeed);
    ry = normalizeAngle(ry);
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

// -------------------------------
// Jumping and Physics
// -------------------------------

void GameObject::handleJumping() {
    float timeStep = 1.0f / 30.0f; // Time step assuming 30 FPS

    // Update vertical velocity and position
    yVelocity -= 9.81f * timeStep; // Gravity effect on velocity
    y += yVelocity * timeStep; // Update vertical position

    // Calculate horizontal movement based on jump direction
    float normX = jumpDirection.x; // Assuming jumpDirection is normalized
    float normZ = jumpDirection.y; // Assuming jumpDirection is normalized
    x += jumpSpeed * normX * timeStep;
    z += jumpSpeed * normZ * timeStep;

    // Check for landing
    if (hasLanded()) {
        resetJumpState();
    }
}

bool GameObject::hasLanded() {
    return y <= 0;
}

void GameObject::resetJumpState() {
    y = 0;
    yVelocity = 0;
    isJumping = false;
    prepareNewJump = true;
}

void GameObject::handleHorizontalMovement(float distance) {
    // if (isJumping && y > 0) return; // Do not move horizontally if in mid-air

    float desiredRy = calculateDesiredRotation(dx, dz);
    rotateTowards(desiredRy);

    if (shouldMoveForward(desiredRy)) {
        moveForward(dx, dz, distance);
    }
    else {
        turnIterations++;
    }
}

bool GameObject::shouldMoveForward(float desiredRy) {
    return fabs(ry - desiredRy) < 0.2 || turnIterations > maxIterations;
}

void GameObject::moveForward(float dx, float dz, float distance) {
    x += speed * dx / distance;
    z += speed * dz / distance;
    turnIterations = 0;
}

void GameObject::jump() {
    if (!isJumping) {
        yVelocity = 0.5f; // Initial jump velocity, adjust as needed
        isJumping = true;
    }
}

/**
 * Determines whether the game object should jump based on a given percentage.
 * 
 * @param percentage The percentage chance (1 to 100) of the game object jumping.
 * @return True if the game object should jump, false otherwise.
 */
bool GameObject::shouldJump(int percentage) {
    if (percentage < 1 || percentage > 100) {
        throw std::invalid_argument("Percentage must be between 1 and 100.");
    }
    return (rand() % 100) < percentage;
}

/**
 * Generates a random direction vector for the game object to jump in.
 * 
 * @return A random direction vector, uniformly distributed.
 */
vec2 GameObject::randomDirection() {
    // Generate a random angle between 0 and 2*PI radians
    float angle = (float)rand() / (float)RAND_MAX * 2.0f * M_PI;

    // Create the direction vector from the angle
    vec2 direction;
    direction.x = cos(angle);  // cos(theta) for the x-component
    direction.y = sin(angle);  // sin(theta) for the z-component

    return direction;
}

void GameObject::startJump(float jumpHeight) {
    float gravity = 9.81f; // Gravity in meters per second squared
    yVelocity = sqrt(2 * gravity * jumpHeight); // Calculate initial jump velocity for the desired height
    isJumping = true; // Set jumping state to true
}






