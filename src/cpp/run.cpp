// uses framework Cocoa
// uses framework OpenGL
#define MAIN
#define NEAR 1.0
#define FAR 1000.0
#define RIGHT 0.5
#define LEFT -0.5
#define TOP 0.5
#define BOTTOM -0.5
#define PI 3.14159265

#include "../ext/Linux/MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "../SimpleGUI/SimpleGUI.h"
#include "../h/Camera.h"
#include "../h/InputController.h"
#include "../h/Terrain.h"
#include "../h/GameMode.h"
#include "../h/GUI.h"

void readColorOfTerrain();

// Variables
// Time based frames
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
// Initial camera settings
vec3 cameraPosition = vec3(0.0f, 5.0f, 8.0f);
vec3 forwardVector = vec3(2.0f, 0.0f, 2.0f);
vec3 cameraUpVector = vec3(0, 1, 0);
GLfloat cameraSpeed = 0.01f;

// Camera object
Camera* camera = new Camera(cameraPosition, forwardVector, cameraUpVector, cameraSpeed);
// Terrain object
Terrain* terrain = new Terrain();
// Input controller object
InputController* inputController = new InputController(camera, terrain);
// Gui object
GUI* gui = new GUI();

// Reference to shader program
GLuint program;

// Texture data
GLuint splat1, splat2, splat3, map;
GLuint tex1, tex2;
Model *tm;

mat4 worldToView, lookAtVectors;

GLfloat projectionMatrix[] = 
{
	2.0f*NEAR/(RIGHT-LEFT), 0.0f, (RIGHT+LEFT)/(RIGHT-LEFT), 0.0f,
	0.0f, 2.0f*NEAR/(TOP-BOTTOM), (TOP+BOTTOM)/(TOP-BOTTOM), 0.0f,
	0.0f, 0.0f, -(FAR+NEAR)/(FAR-NEAR), -2*FAR*NEAR/(FAR-NEAR),
	0.0f, 0.0f, -1.0f, 0.0f
};

/* ----------------------- GUI BEGIN ------------------------ */

float testr, testg, testb, testf, testRef = 5;
int testboolean;
int testradio;
char testString[1024] = "Type here";

float myvec3[3];

void HitButton()
{
	testr = 0;
	testg = 0;
	testb = 0;
}

void SetToRed()
{
	testr = 1;
	testg = 0;
	testb = 0;
}

void SetToGreen()
{
	testr = 0;
	testg = 1;
	testb = 0;
}

void SetToBlue()
{
	testr = 0;
	testg = 0;
	testb = 1;
}

void readColorOfTerrain()
{
	TextureData* texas = terrain->getTextureData();
	// Loop over texas and print out the color of each pixel
	for(int i = 0; i < texas->width * texas->height; i++)
	{
		printf("Color of pixel %d: %d\n", i, texas->imageData[i]);
	}
	printf("Width: %d, Height: %d\n", texas->width, texas->height);
	printf("Total amount of pixels: %d\n", texas->width * texas->height);
	printf("Total amount of vertices in tm: %d\n", tm->numVertices);
	printf("tm->numIndices: %d\n", tm->numIndices);
}

void updateSplat(void)
{
	// Update splat map
	glActiveTexture(GL_TEXTURE3);
	LoadTGATextureSimple("splatmap123.tga", &map);
	glBindTexture(GL_TEXTURE_2D, map);
	glUniform1i(glGetUniformLocation(program, "map"), 3); // Texture unit 3
}


void onMouse(int button, int state, int x, int y) {
  if(state != GLUT_DOWN)
    return;

  GLbyte color[4];
  GLfloat depth;
  GLuint index;
  
  glReadPixels(x, 1080 - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
  glReadPixels(x, 1080 - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
  glReadPixels(x, 1080 - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

  printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
         x, y, color[0], color[1], color[2], color[3], depth, index);
}

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	glUseProgram(program);
	printError("init shader");

	// Load textures
	LoadTGATextureSimple("textures/maskros512.tga", &tex1);
	LoadTGATextureSimple("textures/rock.tga", &tex2);

	// Bind and activate textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);

	// Upload texture units on GPU
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "maskrosTexture"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "rockTexture"), 1); // Texture unit 1

	/*
	SPLATMAP
	LoadTGATextureSimple("textures/grass.tga", &splat1);
	LoadTGATextureSimple("textures/dirt.tga", &splat2);
	LoadTGATextureSimple("textures/conc.tga", &splat3);
	LoadTGATextureSimple("textures/splatmap123.tga", &map);
	*/

	// Bind and activate textures
	/*
	SPLATMAP
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, splat1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, splat2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, splat3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, map);
	*/

	// Upload texture units on GPU
	/*
	glUniform1i(glGetUniformLocation(program, "grass"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "dirt"), 1); // Texture unit 1
	glUniform1i(glGetUniformLocation(program, "conc"), 2); // Texture unit 2
	glUniform1i(glGetUniformLocation(program, "map"), 3); // Texture unit 3
	glUniform1i(glGetUniformLocation(program, "opt"), 1);
	*/

	
	// Load terrain model
	tm = terrain->setTerrainModel("terrain/fft-terrain.tga");
	printError("init terrain!");

	// Init GUI
	gui->initTerrainGUI(terrain);
}

void display(void)
{
	// Update time based frames
	GLfloat currentFrame = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	inputController->handleKeyboardInput(deltaTime);

	/* TESTING */
	Item* hitItem = getHitItem();
	if(hitItem != NULL)
	{
		printf("Hit item position x = %d, y = %d: \n", hitItem->x, hitItem->y);
	}
	/* TESTING */

	// Update height of terrain
	terrain->updateTerrain();

	
	printError("pre display!");

	glUseProgram(program);

	// Upload world-to-view matrix
	worldToView = lookAtv(camera->getPosition(), camera->getPosition() + camera->getForwardVector(), camera->getUpVector()); // camera position, look at point, up vector
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE, worldToView.m);

	// Upload camera position
	GLfloat x = camera->getPosition().x;
	GLfloat y = camera->getPosition().y;
	GLfloat z = camera->getPosition().z;
	glUniform3f(glGetUniformLocation(program, "cameraPosition"), x, y, z);
	// Upload light sources
	glUniform3f(glGetUniformLocation(program, "lightPosition"), 5, 5, 5);
	// Send mountain height
	glUniform1f(glGetUniformLocation(program, "mountainHeight"), terrain->currentMountainHeight);

	// Draw terrain
	tm = terrain->getTerrainModel();
	PlaceModel(tm, program, 0, 0, 0, 0, 0, 0);
	DrawModel(tm, program, "in_Position", "in_Normal", "in_TexCoord");

	/* ------------- GUI ------------------ */
	gui->drawGUI();
	/* ------------- GUI ------------------ */

	printError("display 2");
	
	glutSwapBuffers();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1980, 1020);
	glutCreateWindow ("TSBK07 Project");
	glutDisplayFunc(display);
	// glutPassiveMotionFunc(InputController::handleMouseMotionBridge);
	glutKeyboardFunc(InputController::guiKeyboardBridge);
	glutMouseFunc(InputController::guiMouseBridge);
	glutMotionFunc(InputController::guiDragBridge);
	//glutMouseFunc(onMouse);
	init();
	glutRepeatingTimer(20);
	glutMainLoop();
	exit(0);
}
