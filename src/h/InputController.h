#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "../../ext/Linux/MicroGlut.h"
#include "../SimpleGUI/SimpleGUI.h"
#include "Camera.h"
#include "Terrain.h"
#include "Utils.h"
#include "Picking.h"

class Camera;
class Terrain;
class Picking;

class InputController {
public:

    InputController(Camera* camera, Terrain* terrain, Picking* picker);
    Camera* camera;
    Terrain* terrain;
    Picking* picker;
    void handleKeyboardInput(GLfloat deltaTime);
    void handleMouseMotion(int x, int y);
    void guiKeyboard(unsigned char key, int x, int y);
    void guiMouse(int button, int state, int x, int y);
    void guiDrag(int x, int y);
    void onMouse(int button, int state, int x, int y);

    void cameraControls(GLfloat deltaTime, Camera* camera);
    void placeObjectMode(Picking* picker);
    void terrainEditMode(Picking* picker, Terrain* terrain);

    // Callabacks/bridge functions
    static void handleMouseMotionBridge(int x, int y);
    static void guiKeyboardBridge(unsigned char key, int x, int y);
    static void guiMouseBridge(int button, int state, int x, int y);
    static void guiDragBridge(int x, int y);
    static void onMouseBridge(int button, int state, int x, int y);
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
