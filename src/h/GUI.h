#ifndef GUI_H
#define GUI_H

#include "../SimpleGUI/SimpleGUI.h"
#include "../h/Terrain.h"
#include "../../ext/Linux/MicroGlut.h"

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
    static void RockButton();
    static void GrassButton();
    static void ConcreteButton();



    static bool PlaceBunny;
    static bool manualElevation;
    static bool editTerrainTexture;
    static bool bSwitchGUI;
    static GLubyte textureColor[4];

private:

};

#endif