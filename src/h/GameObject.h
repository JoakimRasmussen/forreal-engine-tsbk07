#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "LoadTGA.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"

#include "math.h"

class GameObject {
public:
    Model* model; // The model of the object

    GameObject(Model* model, int objectID, float x, float y, float z, float rx, float ry, float rz, bool sleeping = false);
    ~GameObject();
    void move(float dx, float dy, float dz);
    void newDestination();
    void moveTowardsDestination();
    void updateAlignmentToTerrain(vec3 normal);
    void setPosition(float x, float y, float z);
    vec3 getPosition() const;
    void setRotation(float rx, float ry, float rz);
    vec3 getRotation() const;
    void jump();

    Model* getModel() const {
        return model;
    }
    int getID() const {
        return objectID;
    }

private:
    int objectID;
    /* y isnt used */
    float x, y, z;
    float targetX, targetZ;
    float rx, ry, rz;
    float speed = 0.1f;
    float turnSpeed = 0.05f;
    int turnIterations = 0;
    int max_iterations = 100;
    bool destinationReached = true;
    bool sleeping = false;

    float yVelocity = 0.0f; // Vertical velocity
    int jumpTime = 0;
    bool isJumping = false;
    
    float lerpAngle(float from, float to, float t);
    float normalizeAngle(float angle);
};

#endif // GAMEOBJECT_H