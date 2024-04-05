#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "LoadTGA.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"

class GameObject {
public:
    Model* model; // The model of the object
    float x, y, z; // Position of the object

    void move(float dx, float dy, float dz);

    GameObject(Model* model, float x, float y, float z)
        : model(model), x(x), y(y), z(z) {}

    Model* getModel() const {
        return model;
    }
    // Add any other methods your game object might need here
};

#endif // GAMEOBJECT_H