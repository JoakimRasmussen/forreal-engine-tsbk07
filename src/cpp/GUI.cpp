#include "../h/GUI.h"

// Define PlaceBunny
bool GUI::PlaceBunny = false;
bool GUI::manualElevation = false;

GUI::GUI() {
    // Constructor implementation
	PlaceBunny = false;
}

void GUI::initTerrainGUI(Terrain* terrain)
{

	sgCreateStaticString(400, 160, "This is the terrain GUI");
	sgSetScale(1);

	/* sgCreateStaticString(40, 220, "Slider color group");
	sgCreateSliderColorGroup(20, 240, 150, &testr, &testg, &testb); */

	// Create a color clicker in the middle of the screen in black
	// myCreateGrayScaledColorPalette(40, 500, &testr, &testg, &testb);

	/* Item* ii = myCreateColorClicker(40, 500, 0, 0, 0, &testr, &testg, &testb);
	Item** items = myGetItems();
	Item* iii = myCreateColorClicker(40, 600, 0, 256, 0, &testr, &testg, &testb); */

	sgCreateStaticString(40, 20, "Elevation slider"); sgCreateButton(500, 20, "Manual Elevation", ElevationButton);	
	sgCreateSlider(40, 40, 200, &terrain->currentElevation, 1, 20);
	sgCreateDisplayFloat(40, 60, "Elevation value: ", &terrain->currentElevation);

	sgCreateStaticString(40, 80, "--------------------------");

	sgCreateStaticString(40, 100, "Mountain slider"); 
	sgCreateSlider(40, 120, 200, &terrain->currentMountainHeight, 1, 500);
	sgCreateDisplayFloat(40, 140, "Mountain value: ", &terrain->currentMountainHeight);

	sgCreateStaticString(40, 160, "--------------------------");

	sgCreateStaticString(40, 180, "Smooth slider");
	sgCreateSlider(40, 200, 200, &terrain->quadSize, 1, 20);
	sgCreateDisplayFloat(40, 220, "Smooth value: ", &terrain->quadSize);

	sgCreateStaticString(40, 240, "--------------------------");
	sgCreateButton(40, 260, "Place a Bunny", PlaceBunnyButton);
}

void GUI::drawGUI()
{
	sgDraw();
}

void GUI::PlaceBunnyButton()
{
	if (!PlaceBunny)
	{
		PlaceBunny = true;
	}
}

void GUI::ElevationButton()
{
	if (manualElevation)
	{
		manualElevation = false;
	}
	else
	{
		manualElevation = true;
	}
}