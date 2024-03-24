#include "../h/GUI.h"


GUI::GUI() {
    // Constructor implementation
}

void GUI::initTerrainGUI(Terrain* terrain)
{

	sgCreateStaticString(400, 160, "This is the terrain GUI");
	sgSetScale(2);

	/* sgCreateStaticString(40, 220, "Slider color group");
	sgCreateSliderColorGroup(20, 240, 150, &testr, &testg, &testb); */

	// Create a color clicker in the middle of the screen in black
	// myCreateGrayScaledColorPalette(40, 500, &testr, &testg, &testb);

	/* Item* ii = myCreateColorClicker(40, 500, 0, 0, 0, &testr, &testg, &testb);
	Item** items = myGetItems();
	Item* iii = myCreateColorClicker(40, 600, 0, 256, 0, &testr, &testg, &testb); */

	sgCreateStaticString(40, 20, "Elevation slider");
	sgCreateSlider(40, 40, 200, &terrain->currentElevation, 1, 20);
	sgCreateDisplayFloat(40, 60, "Elevation value: ", &terrain->currentElevation);
}

void GUI::drawGUI()
{
	sgDraw();
}