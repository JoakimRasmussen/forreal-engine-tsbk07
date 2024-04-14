#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "LoadTGA.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"

class GameObject {
public:
    Model* model; // The model of the object

    GameObject(Model* model, float x, float y, float z, float rx, float ry, float rz);

    void move(float dx, float dy, float dz);
    void newDestination();
    void moveTowardsDestination();


    void updateAlignment(vec3 normal, vec3 toCamera, bool forceRotation = false);
    void setPosition(float x, float y, float z);
    vec3 getPosition() const;
    void setRotation(float rx, float ry, float rz);
    vec3 getRotation() const;
    float lerpAngle(float from, float to, float t);

    // GameObject(Model* model, float x, float y, float z, float rx, float ry, float rz)
    //         : model(model), x(x), y(y), z(z), rx(rx), ry(ry), rz(rz) {}
    
    Model* getModel() const {
        return model;
    }

private:
    /* y isnt used */
    float x, y, z;
    // float lastx, lasty, lastz;
    float targetX, targetZ;
    float rx, ry, rz;
    bool destinationReached = true;

    float vy; // Vertical velocity
    float gravity; // Gravity affecting the jump
    bool isJumping; // Is the bunny currently jumping
};

#endif // GAMEOBJECT_H