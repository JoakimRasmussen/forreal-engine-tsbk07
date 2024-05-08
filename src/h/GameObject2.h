#ifndef GAMEOBJECT2_H
#define GAMEOBJECT2_H

#include <vector>
#include "GL_utilities.h"
#include "LoadTexture.h"
#include "LittleOBJLoader.h"
#include "Utils.h"
#include "Terrain.h"

class Terrain;

class GameObject2
{
    public:

        // Constructor
        GameObject2(Model* model, Terrain* terrain, int objectID);
        // Destructor
        //~GameObject2();

        void renderGameObject(GLuint& shaderProgram, bool pickingPhase = false);
        void setTexture(GLuint& texture, GLuint textureUnit);

        void setPosition(GLfloat x, GLfloat z){
            this->posX = x;
            this->posZ = z;
        }
        vec3 getPosition() const{
            return vec3(posX, posY, posZ);
        }

        int getObjectID() const{
            return objectID;
        }

        // GLuint shaderProgram;
        GLuint furTex, textureUnit;
        Model* model;
        Terrain* terrain;

        GLfloat posX, posZ;
        GLfloat terrainY, jumpY, posY; // posY is the sum of terrainY and jumpY
        GLfloat yOffset = 0.6f;
        GLfloat rotX, rotY, rotZ;
        mat4 modelToWorldMatrix;

        int objectID;

    private:

        // void bunnyMovement();
        // void updateJump();
        // bool decideJumpByPercentage(int percentage);
        // vec2 getRandomDirection();
        // bool hasLanded();
        // void resetJumpState();
        // void rotateTowardsDirection(vec2 direction);
        // bool rotationComplete();

        bool jumpingPhase = false;
        bool prepareNewJump = true;
        vec2 jumpDirection;
        int turnIterations = 0;
        int maxIterations = 100;
        int jumpPercentage = 1; // 1% chance of jumping

        
    
};

#endif // GAMEOBJECT2_H