#include "../h/GameObject.h"

// Constructor
GameObject::GameObject(Model* model, Terrain *terrain, int objectID) {
    this->terrain = terrain;
    printError("GameObject - terrain");
    this->objectID = objectID;
    printError("GameObject - objectID");
    this->model = model;
    printError("GameObject - model");
    this->modelToWorldMatrix = IdentityMatrix();

    // Initialize the object's position
    this->posX = 0.0f;
    this->posZ = 0.0f;
    this->terrainY = 0.0f;
    this->offsetY = 0.6f;
    this->posY = 0.0f;

    // Initialize the object's rotation
    this->rotX = 0.0f;
    this->rotY = 0.0f;
    this->rotZ = 0.0f;

    // Initialize the object's texture
    this->objectTexture = 0;
    this->textureUnit = 0;

    // Initialize the object's jump state
    this->jumpDirection = vec2(0.0f, 0.0f);

    // Initialize the object's jump physics
    this->jumpSpeed = 4.0f;
    this->jumpHeight = 2.0f;
    this->gravity = 9.81f;

    // Initialize the object's jump behavior
    this->jumpPercentage = 1;
    this->turnIterations = 0;
    this->maxIterations = 100;

    // Initialize the object's movement behavior
    this->resetJumpState();

}

void GameObject::renderGameObject(GLuint& shaderProgram, bool pickingPhase)
{
    // Update the position of the object
    this->terrainY = terrain->getHeightAtPoint(this->posX, this->posZ);
    this->posY = terrainY + jumpY + offsetY;

    // Align the object to the terrain
    vec3 normal = terrain->getNormalAtPoint(posX, posZ);
    this->rotX = atan2(normal.z, normal.y);

    // Update the modelToWorldMatrix
    this->modelToWorldMatrix = T(posX, posY, posZ) * Rx(rotX) * Ry(rotY) * Rz(rotZ);

    // Upload the modelToWorldMatrix to the shader
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelToWorld"), 1, GL_TRUE, modelToWorldMatrix.m);

    // // Upload the fur texture to the shader
    // glActiveTexture(GL_TEXTURE0 + textureUnit);
    // // glActiveTexture(GL_TEXTURE4);
    // glBindTexture(GL_TEXTURE_2D, objectTexture);

    // Render the object
    if (pickingPhase) {
        DrawModel(model, shaderProgram, "in_Position", NULL, NULL);
    }
    else {
        DrawModel(model, shaderProgram, "in_Position", "in_Normal", "in_TexCoord");
    }
}

void GameObject::setTexture(GLuint& texture, GLuint textureUnit)
{
    this->objectTexture = texture;
    this->textureUnit = textureUnit;
}

void GameObject::bunnyMovement()
{
    // Check if the object is jumping
    if (jumpingPhase) {
        updateJump();
        return;
    }

    // Select new random direction
    if (prepareNewJump) {
        if (!isChanceSuccessful(jumpPercentage)) {
        return;
        }
        jumpDirection = getRandomDirection();
        prepareNewJump = false;
    }

    // Rotate smoothly towards the new direction
    rotateTowardsDirection(jumpDirection);
    if (rotationComplete()) {
        turnIterations = 0;
        jumpingPhase = true;
    }
}

void GameObject::updateJump()
{
    float timeStep = 1.0f / 30.0f;
    yVelocity -= gravity * timeStep;
    jumpY += yVelocity * timeStep;

    posX += jumpSpeed * jumpDirection.x * timeStep;
    posZ += jumpSpeed * jumpDirection.y * timeStep;

    if (hasLanded()) {
        updateAlignmentToTerrain();
        resetJumpState();
        return;
    }
}

bool GameObject::hasLanded() {
    return jumpY<= 0;
}

void GameObject::resetJumpState() {
    jumpY = 0;
    yVelocity = sqrt(2 * gravity * jumpHeight);
    jumpingPhase = false;
    prepareNewJump = true;
}

/**
 * Determines whether the game object should perform an action based on a given percentage.
 * 
 * @param percentage The percentage chance (1 to 100) of the game object performing an action.
 * @return True if the game object should perform the action, false otherwise.
 */
bool GameObject::isChanceSuccessful(int percentage) {
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
vec2 GameObject::getRandomDirection() {
    // Generate a random angle between 0 and 2*PI radians
    float angle = (float)rand() / (float)RAND_MAX * 2.0f * M_PI;

    // Create the direction vector from the angle
    vec2 direction;
    direction.x = cos(angle);  // cos(theta) for the x-component
    direction.y = sin(angle);  // sin(theta) for the z-component

    return direction;
}

void GameObject::rotateTowardsDirection(vec2 direction) {
    float angle = atan2(direction.x, direction.y);
    angle = normalizeAngle(angle);
    rotY = lerpAngle(rotY, angle, 0.05f);
    rotY = normalizeAngle(rotY);
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

float GameObject::lerpAngle(float from, float to, float speed) {
    float difference = to - from;
    if (difference > M_PI) {
        difference -= 2 * M_PI;  // Adjust for wrap-around
    } else if (difference < -M_PI) {
        difference += 2 * M_PI;  // Adjust for wrap-around
    }

    float adjusted = from + difference * speed;
    
    // Normalize the result to keep within -PI to PI
    if (adjusted > M_PI) {
        adjusted -= 2 * M_PI;
    } else if (adjusted < -M_PI) {
        adjusted += 2 * M_PI;
    }
    return adjusted;
}

bool GameObject::rotationComplete() {
    float endAngle = normalizeAngle(atan2(jumpDirection.x, jumpDirection.y));
    float angleCheck = fabs(rotY - endAngle) < 0.2f;
    bool iterationCheck = turnIterations++ > maxIterations;
    return angleCheck || iterationCheck;
}

void GameObject::updateAlignmentToTerrain() {
    vec3 normal = terrain->getNormalAtPoint(posX, posZ);
    rotX = atan2(normal.z, normal.y);
}