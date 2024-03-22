#ifndef GAME_MODE_H
#define GAME_MODE_H

#include "Camera.h"
#include "InputController.h"
#include "Terrain.h"

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
};

#endif
