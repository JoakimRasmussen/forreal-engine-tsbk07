#ifndef GUI_H
#define GUI_H

#include "../SimpleGUI/SimpleGUI.h"
#include "../h/Terrain.h"
#include "../../ext/Linux/MicroGlut.h"

class GUI {

public:

    GUI();
    void initTerrainGUI(Terrain* terrain);
    void initObjectGUI();
    void drawGUI();

    static void PlaceBunnyButton();
    static bool PlaceBunny;
    static void ElevationButton();
    static bool manualElevation;

private:

};

#endif