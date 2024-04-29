#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "LoadTGA.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include <stdexcept>  // Include this for std::invalid_argument
#include "math.h"

class GameObject {
public:
    // Constructors and Destructors
    GameObject(Model* model, int objectID, float x, float y, float z, float rx, float ry, float rz, bool sleeping = false);
    ~GameObject();

    // Publicly Accessible Movement and Logic Functions
    void move(float dx, float dy, float dz);
    void moveTowardsDestination(float distance);
    void bunnyMovement();
    void setPosition(float x, float y, float z);
    vec3 getPosition() const;
    void setRotation(float rx, float ry, float rz);
    vec3 getRotation() const;
    
    void updateAlignmentToTerrain(vec3 normal);
    void jump();
    void logic();

    Model* getModel() const{
        return model;
    }

    int getObjectID() const{
        return objectID;
    }

private:
    // Private Attributes
    int objectID;
    Model* model; // The model of the object
    float x, y, z; // Position coordinates
    float rx, ry, rz; // Rotation coordinates
    float targetX, targetZ; // Target position for movement
    float dx, dz; // Direction for movement
    float speed = 0.1f;
    float turnSpeed = 0.05f;
    int turnIterations = 0;
    int maxIterations = 100;
    bool destinationReached = true;
    bool sleeping = false;

    // Jumping and Physics Related
    float yVelocity = 0.0f; // Vertical velocity
    int jumpTime = 1;
    bool isJumping = false;

    // Private Movement Functions
    void newDestination();

    // Private Helper Functions
    float lerpAngle(float from, float to, float t);
    float normalizeAngle(float angle);
    float distanceToTarget();
    bool isCloseEnough(float distance);
    void handleJumping();
    void handleHorizontalMovement(float distance);
    bool hasLanded();
    void resetJumpState();
    float calculateDesiredRotation(float dx, float dy);
    void rotateTowards(float desiredRy);
    bool shouldMoveForward(float desiredRy);
    void moveForward(float dx, float dy, float distance);

    bool shouldJump(int percentage);
    vec2 randomDirection();
    void startJump(float jumpHeight);
    vec2 jumpDirection;
    bool prepareNewJump = true;
    float jumpSpeed = 4.0f;
};

#endif // GAMEOBJECT_H
