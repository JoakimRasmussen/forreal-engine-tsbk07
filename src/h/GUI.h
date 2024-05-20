#ifndef GUI_H
#define GUI_H

#include "../SimpleGUI/SimpleGUI.h"
#include "../h/Terrain.h"
#include "../../ext/Linux/MicroGlut.h"
#include "../h/Utils.h"

class GUI {

public:

    GUI();
    void initGUI();
    void initTextureGUI();
    void initTerrainGUI(Terrain* terrain);
    void drawGUI();
    bool switchGUI();

    static void PlaceBunnyButton();
    static void ElevationButton();
    static void TextureButton();
    static void DirtButton();
    static void GrassButton();
    static void RockButton();
    static void PlaceBillboardButton();
    static void SmoothButton();


    static bool PlaceBunny;
    static bool manualElevation;
    static bool editTerrainTexture;
    static bool PlaceBillboard;
    static bool smoothTerrain;
    static GLubyte textureColor[4];

    static bool showGUI;

    static int fpsStringID;
    static int fpsX, fpsY;  // Coordinates for the FPS display
    static char fpsText[50];
    static void setupFPSCounter();
    static void updateFPSDisplay(int fps);
    static float lightPosX;

private:

    void adjustWindowScale();
};

#endif