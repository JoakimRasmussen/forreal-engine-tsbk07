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
    float rx, ry, rz; // Rotation of the object

    void move(float dx, float dy, float dz);

    GameObject(Model* model, float x, float y, float z, float rx, float ry, float rz)
            : model(model), x(x), y(y), z(z), rx(rx), ry(ry), rz(rz) {}
    
    Model* getModel() const {
        return model;
    }
    // Add any other methods your game object might need here
};

#endif // GAMEOBJECT_H