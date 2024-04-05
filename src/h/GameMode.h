#ifndef GAME_MODE_H
#define GAME_MODE_H

#include <vector>
#include "Camera.h"
#include "InputController.h"
#include "Terrain.h"
#include "GUI.h"
#include "Utils.h"
#include "GameObject.h"
#include "Picking.h"
#include "GL_utilities.h"

class GameMode {
public:
    GameMode();
    void init();
    void display();
    void run(int argc, char** argv);
    void updateCameraVariables();
    void bunnyButtonLogic();
    void drawGameObjects();
    void manualElevationButton();

private:

    // Time based frames
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // Initial classes
    Camera* camera;
    Terrain* terrain;
    InputController* inputController;
    GUI* gui;
    Utils* utils;
    Picking* picker;

    // Place to store game objects
    std::vector<GameObject> gameObjects;

    // Object variables
    const float placementDistance = 10.0f;

    // Reference to shader program
    GLuint program;

    // Texture data
    GLuint splat1, splat2, splat3, map;
    GLuint tex1, tex2;
    Model *tm, *bunnyModel;

    mat4 worldToView, modelToWorld, lookAtVectors;
    vec3 cameraPos, forwardVec, upVec;

    const GLfloat* projectionMatrix;

};

#endif
