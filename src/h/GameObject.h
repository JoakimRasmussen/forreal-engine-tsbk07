#ifndef GameObject_H
#define GameObject_H

#include <vector>
#include "GL_utilities.h"
#include "LoadTexture.h"
#include "LittleOBJLoader.h"
#include "Utils.h"
#include "Terrain.h"

class Terrain;

class GameObject
{
    public:

        // Constructor
        GameObject(Model* model, Terrain* terrain, int objectID);

        void renderGameObject(GLuint& shaderProgram, bool pickingPhase = false);
        void setTexture(GLuint& texture, GLuint textureUnit);
        void bunnyMovement();

        void setPosition(GLfloat x, GLfloat z){
            this->posX = x;
            this->posZ = z;
            this->terrainY = terrain->getHeightAtPoint(this->posX, this->posZ);
            this->posY = terrainY + jumpY + offsetY;
            updateAlignmentToTerrain();
        }
        vec3 getPosition() const{
            return vec3(posX, posY, posZ);
        }

        int getObjectID() const{
            return objectID;
        }

        bool textureIsSet() const{
            return objectTexture != 0 && textureUnit != 0;
        }

    private:

        void updateJump();
        vec2 getRandomDirection();
        bool hasLanded();
        void resetJumpState();
        void rotateTowardsDirection(vec2 direction);
        bool rotationComplete();
        void updateAlignmentToTerrain();

        GLuint objectTexture, textureUnit;
        Model* model;
        Terrain* terrain;

        GLfloat posX, posZ;
        GLfloat terrainY, jumpY, posY, offsetY; // posY is the sum of terrainY and jumpY (+ offset)
        GLfloat rotX, rotY, rotZ;
        mat4 modelToWorldMatrix;

        int objectID;
        bool jumpingPhase, prepareNewJump;
        vec2 jumpDirection;
        int turnIterations, maxIterations;
        int jumpPercentage = 1; // 1% chance of jumping
        float yVelocity, jumpSpeed, jumpHeight, gravity;
        
    
};

#endif // GameObject_H