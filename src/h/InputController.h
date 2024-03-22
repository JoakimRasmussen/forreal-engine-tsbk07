#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "Camera.h"
#include "Terrain.h"
#include "../../ext/Linux/MicroGlut.h"
#include "Utils.h"

class Camera;
class Terrain;

class InputController {
public:

    InputController(Camera* camera, Terrain* terrain);
    Camera* camera;
    Terrain* terrain;
    void handleKeyboardInput(GLfloat deltaTime);
    void handleMouseMotion(int x, int y);
    static void handleMouseMotionBridge(int x, int y);
    Utils utils;

private:
    // Mouse variables
    bool firstMouse = true;
    GLfloat lastX = 300, lastY = 300; // Screen width and height
    static InputController* instance;
};

#endif
