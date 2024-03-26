#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "LoadTGA.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"

class GameObject {
public:
    float x, y, z; // Position of the object

    void move(float dx, float dy, float dz);

    GameObject(Model* model, float x, float y, float z)
        : x(x), y(y), z(z) {}

    // Add any other methods your game object might need here
};

#endif // GAMEOBJECT_H