#ifndef GUI_H
#define GUI_H

#include "../SimpleGUI/SimpleGUI.h"
#include "../h/Terrain.h"
#include "../../ext/Linux/MicroGlut.h"
#include "../h/Utils.h"

class GUI {

public:

    GUI();
    void initTerrainGUI(Terrain* terrain);
    void initObjectGUI();
    void drawGUI();

    static void PlaceBunnyButton();
    static void ElevationButton();
    static void TextureButton();
    static void DirtButton();
    static void GrassButton();
    static void RockButton();
    static void PlaceBillboardButton();


    static bool PlaceBunny;
    static bool manualElevation;
    static bool editTerrainTexture;
    static bool PlaceBillboard;
    static GLubyte textureColor[4];

private:

    void adjustWindowScale();
    const float baseWidth = 1080.0f;
    const float baseHeight = 720.0f;
    // const float baseWidth = 1920.0f;
    // const float baseHeight = 1080.0f;
};

#endif