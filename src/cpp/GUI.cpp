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
	// sgSetScale(2);
	adjustWindowScale();
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

void GUI::adjustWindowScale() {

    // Calculate scale based on current resolution compared to base resolution
    float scaleX = static_cast<float>(Utils::windowWidth) / baseWidth;
    float scaleY = static_cast<float>(Utils::windowHeight) / baseHeight;

    // Average the scale factors to avoid distortion
    float averageScale = (scaleX + scaleY) / 2.0f;

    // Round the average scale to the nearest integer
    int roundedScale = static_cast<int>(std::round(averageScale));

    // Ensure the scale is at least 1
    roundedScale = std::max(1, roundedScale);

    // Set the scale using your specific function, adjusting the factor as necessary
    sgSetScale(roundedScale);
}

void GUI::PlaceBunnyButton()
{
	if (!PlaceBunny)
	{
		PlaceBunny = true;
		PlaceBillboard = false;
		manualElevation = false;
		editTerrainTexture = false;
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
		manualElevation = false;
		editTerrainTexture = false;
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
		PlaceBunny = false;
		PlaceBillboard = false;
		editTerrainTexture = false;
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
		PlaceBunny = false;
		PlaceBillboard = false;
		manualElevation = false;
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