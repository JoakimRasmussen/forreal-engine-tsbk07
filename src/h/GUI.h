#ifndef GUI_H
#define GUI_H

#include "../SimpleGUI/SimpleGUI.h"
#include "../h/Terrain.h"
#include "../../ext/Linux/MicroGlut.h"

class GUI {

public:

    GUI();
    void initTerrainGUI(Terrain* terrain);
    void drawGUI();

    static void PlaceBunnyButton();
    static bool PlaceBunny;

private:

};

#endif