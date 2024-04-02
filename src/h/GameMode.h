#ifndef GAME_MODE_H
#define GAME_MODE_H


#include "Camera.h"
#include "InputController.h"
#include "Terrain.h"
#include "GUI.h"
#include "Utils.h"

class GameMode {
public:
    GameMode();
    void init();
    void display();
    void run(int argc, char** argv);

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

    // Reference to shader program
    GLuint program;

    // Texture data
    GLuint splat1, splat2, splat3, map;
    GLuint tex1, tex2;
    Model *tm;

    mat4 worldToView, lookAtVectors;

    GLfloat* projectionMatrix;

};

#endif
