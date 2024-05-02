#include "../h/GUI.h"

// Define PlaceBunny
bool GUI::PlaceBunny = false;
bool GUI::manualElevation = false;
bool GUI::editTerrainTexture = false;
bool GUI::PlaceBillboard = false;
GLubyte GUI::textureColor[4] = {0, 0, 0, 255};

GUI::GUI() {
    // Constructor implementation
	PlaceBunny = false;
}

void GUI::initTerrainGUI(Terrain* terrain)
{
	sgSetScale(2);
	sgCreateStaticString(400, 160, "GUI");
	sgCreateStaticString(40, 20, "Elevation slider"); 
	sgCreateSlider(40, 40, 200, &terrain->currentElevation, 1, 20);
	sgCreateDisplayFloat(40, 60, "Elevation value: ", &terrain->currentElevation);
	
	sgCreateButton(500, 20, "Toggle Manual Elevation", ElevationButton);	

	// GUI for manual texture
	sgCreateButton(500, 40, "Toggle Manual Texture", TextureButton);	
	sgCreateButton(500, 60, "Grass texture", GrassButton);
	sgCreateButton(500, 80, "Dirt texture", DirtButton);
	sgCreateButton(500, 100, "Rock texture", RockButton);

	sgCreateStaticString(40, 80, "--------------------------");

	sgCreateStaticString(40, 100, "Smooth slider");
	sgCreateSlider(40, 120, 200, &terrain->quadSize, 1, 20);
	sgCreateDisplayFloat(40, 140, "Smooth value: ", &terrain->quadSize);

	sgCreateStaticString(40, 240, "--------------------------");
	sgCreateButton(40, 260, "Place a Bunny", PlaceBunnyButton);
	sgCreateButton(200, 260, "Toggle Place Plants", PlaceBillboardButton);
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
void GUI::PlaceBillboardButton()
{
	if (PlaceBillboard)
	{
		PlaceBillboard = false;
	}
	else
	{
		PlaceBillboard = true;
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

void GUI::TextureButton()
{
	if (editTerrainTexture)
	{
		editTerrainTexture = false;
	}
	else
	{
		editTerrainTexture = true;
	}
}

void GUI::GrassButton()
{
	printf("Grass texture\n");
	textureColor[0] = 255;
	textureColor[1] = 0;
	textureColor[2] = 0;
}

void GUI::DirtButton()
{
	printf("Dirt texture\n");
	textureColor[0] = 0;
	textureColor[1] = 255;
	textureColor[2] = 0;
}

void GUI::RockButton()
{
	printf("Rock texture\n");
	textureColor[0] = 0;
	textureColor[1] = 0;
	textureColor[2] = 255;
}