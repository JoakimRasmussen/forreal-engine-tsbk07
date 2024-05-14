#include "../h/GameObject.h"

// Constructor
GameObject::GameObject(Model* model, Terrain *terrain, int objectID) {
    this->terrain = terrain;
    this->objectID = objectID;
    this->model = model;
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

    // Upload the fur texture to the shader
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, objectTexture);
    glUniform1i(glGetUniformLocation(shaderProgram, "objectTexture"), textureUnit);

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
        if (!Utils::isChanceSuccessful(jumpPercentage)) {
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
    angle = Utils::normalizeAngle(angle);
    rotY = Utils::lerpAngle(rotY, angle, 0.05f);
    rotY = Utils::normalizeAngle(rotY);
}

bool GameObject::rotationComplete() {
    float endAngle = Utils::normalizeAngle(atan2(jumpDirection.x, jumpDirection.y));
    float angleCheck = fabs(rotY - endAngle) < 0.2f;
    bool iterationCheck = turnIterations++ > maxIterations;
    return angleCheck || iterationCheck;
}

void GameObject::updateAlignmentToTerrain() {
    vec3 normal = terrain->getNormalAtPoint(posX, posZ);
    rotX = atan2(normal.z, normal.y);
}