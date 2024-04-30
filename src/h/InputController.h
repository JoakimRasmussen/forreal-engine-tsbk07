#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "../../ext/Linux/MicroGlut.h"
#include "../SimpleGUI/SimpleGUI.h"
#include "Camera.h"
#include "Terrain.h"
#include "Utils.h"
#include "Picking.h"
#include "Billboard.h"
#include "GUI.h"

class Camera;
class Terrain;
class Picking;
class Billboard;

class InputController {
public:

    InputController(Camera* camera, Terrain* terrain, Picking* picker, Billboard* billboard);
    Camera* camera;
    Terrain* terrain;
    Picking* picker;
    Billboard* billboard;
    void handleKeyboardInput(GLfloat deltaTime);
    void handleMouseMotion(int x, int y);
    void guiKeyboard(unsigned char key, int x, int y);
    void guiDrag(int x, int y);
    void onMouse(int button, int state, int x, int y);
    void collectedMouseController(int button, int state, int x, int y);

    void cameraControls(GLfloat deltaTime, Camera* camera);

    // getHitCoordinates
    int getHitX() const {
        return hitx;
    }
    int getHitY() const {
        return hity;
    }

    // Reset hit coordinates
    void resetHitCoordinates() {
        hitx = -1;
        hity = -1;
    }

    // Callabacks/bridge functions
    static void handleMouseMotionBridge(int x, int y);
    static void guiKeyboardBridge(unsigned char key, int x, int y);
    static void guiDragBridge(int x, int y);
    static void collectedMouseControllerBridge(int button, int state, int x, int y);
    Utils utils;


private:
    // Mouse variables
    bool firstMouse = true;
    GLfloat lastX = 300, lastY = 300; // Screen width and height
    int hitx = -1, hity = -1;
    static InputController* instance;
    // Test string
    char testString[1024] = "Type here";

};

#endif
