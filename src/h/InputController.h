#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "Camera.h"
#include "Terrain.h"
#include "../../ext/Linux/MicroGlut.h"
#include "Utils.h"
#include "../SimpleGUI/SimpleGUI.h"

class Camera;
class Terrain;

class InputController {
public:

    InputController(Camera* camera, Terrain* terrain);
    Camera* camera;
    Terrain* terrain;
    void handleKeyboardInput(GLfloat deltaTime);
    void handleMouseMotion(int x, int y);
    void guiKeyboard(unsigned char key, int x, int y);
    void guiMouse(int button, int state, int x, int y);
    void guiDrag(int x, int y);
    // Callabacks/bridge functions
    static void handleMouseMotionBridge(int x, int y);
    static void guiKeyboardBridge(unsigned char key, int x, int y);
    static void guiMouseBridge(int button, int state, int x, int y);
    static void guiDragBridge(int x, int y);
    Utils utils;

private:
    // Mouse variables
    bool firstMouse = true;
    GLfloat lastX = 300, lastY = 300; // Screen width and height
    static InputController* instance;
    // Test string
    char testString[1024] = "Type here";
};

#endif
