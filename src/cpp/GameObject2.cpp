#include "../h/GameObject2.h"

// Constructor
GameObject2::GameObject2(Model* model, Terrain *terrain, int objectID) {
    printf("Creating GameObject2\n");
    this->terrain = terrain;
    printError("GameObject2 - terrain");
    this->objectID = objectID;
    printError("GameObject2 - objectID");
    this->model = model;
    printError("GameObject2 - model");
    this->modelToWorldMatrix = IdentityMatrix();
}

// Destructor
/* GameObject2::~GameObject2() {
    // Delete the game object model
    delete this->model;
} */

void GameObject2::renderGameObject(GLuint& shaderProgram, bool pickingPhase)
{
    // Update the position of the object
    this->terrainY = terrain->getHeightAtPoint(this->posX, this->posZ);
    this->jumpY = 0.0f;
    this->posY = terrainY + jumpY + yOffset;

    // Align the object to the terrain
    vec3 normal = terrain->getNormalAtPoint(posX, posZ);
    this->rotX = atan2(normal.z, normal.y);

    // Update the modelToWorldMatrix
    this->modelToWorldMatrix = T(posX, posY, posZ) * Rx(rotY) * Ry(rotX) * Rz(rotZ);
    printError("renderGameObject - modelToWorldMatrix");

    // Upload the modelToWorldMatrix to the shader
    glValidateProgram(shaderProgram);
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelToWorld"), 1, GL_TRUE, modelToWorldMatrix.m);
    printError("renderGameObject - Upload of modelToWorldMatrix");

    // Upload the fur texture to the shader
    // glActiveTexture(GL_TEXTURE0 + textureUnit);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, furTex);
    printError("renderGameObject - Upload of furTex");

    // Render the object
    DrawModel(model, shaderProgram, "in_Position", "in_Normal", "in_TexCoord");
    printError("renderGameObject - DrawModel");
}

void GameObject2::setTexture(GLuint& texture, GLuint textureUnit)
{
    this->furTex = texture;
    this->textureUnit = textureUnit;
}

// void GameObject2::bunnyMovement()
// {
//     // Check if the object is jumping
//     if (jumpingPhase) {
//         updateJump();
//         return;
//     }

//     // Select new random direction
//     if (prepareNewJump) {
//         if (!decideJumpByPercentage(jumpPercentage)) {
//         return;
//         }
//         jumpDirection = getRandomDirection();
//         prepareNewJump = false;
//     }

//     // Rotate smoothly towards the new direction
//     rotateTowardsDirection(jumpDirection);
//     if (rotationComplete()) {
//         turnIterations = 0;
//         jumpingPhase = true;
//     }
// }