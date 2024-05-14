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
    void guiDrag(int x, int y);
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

    // Callbacks/bridge functions
    static void guiDragBridge(int x, int y);
    static void collectedMouseControllerBridge(int button, int state, int x, int y);
    Utils utils;

private:

    // Mouse variables
    int hitx = -1, hity = -1;
    static InputController* instance;
    bool hKeyWasDown;
};

#endif
