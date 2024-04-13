#include "../h/GUI.h"

// Define PlaceBunny
bool GUI::PlaceBunny = false;
bool GUI::manualElevation = false;
bool GUI::editTerrainTexture = false;
bool GUI::bSwitchGUI = false;
GLubyte GUI::textureColor[4] = {0, 0, 0, 255};

GUI::GUI() {
    // Constructor implementation
	PlaceBunny = false;
}

void GUI::initGUI()
{
	sgSetScale(1);
	sgCreateStaticString(400, 160, "This is the terrain GUI");
	sgCreateButton(500, 20, "Manual Elevation Off", ElevationButton);
	sgCreateButton(500, 40, "Manual Texture Off", TextureButton);

	sgCreateStaticString(40, 240, "--------------------------");
	sgCreateButton(40, 260, "Place Bunny", PlaceBunnyButton);
}

void GUI::initTextureGUI()
{
	sgSetScale(1);
	sgCreateStaticString(400, 160, "This is the texture GUI");
	sgCreateButton(500, 40, "Manual Texture On", TextureButton);
	sgCreateButton(500, 60, "Grass texture", RockButton);
	sgCreateButton(500, 80, "Rock texture", GrassButton);
	sgCreateButton(500, 100, "Concrete texture", ConcreteButton);
}

void GUI::initTerrainGUI(Terrain* terrain)
{
	sgSetScale(1);
	sgCreateStaticString(400, 160, "This is the terrain GUI");

	sgCreateStaticString(40, 20, "Elevation slider"); 
	sgCreateSlider(40, 40, 200, &terrain->currentElevation, 1, 20);
	sgCreateDisplayFloat(40, 60, "Elevation value: ", &terrain->currentElevation);

	sgCreateButton(500, 20, "Manual Elevation On", ElevationButton);	

	sgCreateStaticString(40, 80, "--------------------------");

	sgCreateStaticString(40, 100, "Mountain slider"); 
	sgCreateSlider(40, 120, 200, &terrain->currentMountainHeight, 1, 500);
	sgCreateDisplayFloat(40, 140, "Mountain value: ", &terrain->currentMountainHeight);

	sgCreateStaticString(40, 160, "--------------------------");

	sgCreateStaticString(40, 180, "Smooth slider");
	sgCreateSlider(40, 200, 200, &terrain->quadSize, 1, 20);
	sgCreateDisplayFloat(40, 220, "Smooth value: ", &terrain->quadSize);
}

bool GUI::switchGUI()
{
	return bSwitchGUI;
}

void GUI::drawGUI()
{
	sgDraw();
}

void GUI::PlaceBunnyButton()
{
	if (PlaceBunny)
	{
		PlaceBunny = false;
		bSwitchGUI = true;
	}
	else
	{	
		PlaceBunny = true;
		bSwitchGUI = true;
	}
}

void GUI::ElevationButton()
{
	if (manualElevation)
	{
		manualElevation = false;
		bSwitchGUI = true;
	}
	else
	{
		manualElevation = true;
		bSwitchGUI = true;
	}
}

void GUI::TextureButton()
{
	if (editTerrainTexture)
	{
		editTerrainTexture = false;
		bSwitchGUI = true;
	}
	else
	{
		editTerrainTexture = true;
		bSwitchGUI = true;
	}
}

void GUI::GrassButton()
{
	printf("Gras texture\n");
	textureColor[0] = 0;
	textureColor[1] = 255;
	textureColor[2] = 0;
}

void GUI::RockButton()
{
	printf("Rock texture\n");
	textureColor[0] = 255;
	textureColor[1] = 0;
	textureColor[2] = 0;
}

void GUI::ConcreteButton()
{
	printf("Concrete texture\n");
	textureColor[0] = 0;
	textureColor[1] = 0;
	textureColor[2] = 255;
}