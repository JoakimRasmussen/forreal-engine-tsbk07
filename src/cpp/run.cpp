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

#include <vector>
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
#include "../h/GameObject.h"

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
Model *bunnyModel;
Model *sphereModel;

std::vector<GameObject> gameObjects;

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
	for(GLuint i = 0; i < texas->width * texas->height; i++)
	{
		printf("Color of pixel %u: %d\n", i, texas->imageData[i]);
	}
	printf("Width: %d, Height: %d\n", texas->width, texas->height);
	printf("Total amount of pixels: %u\n", texas->width * texas->height);
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

/* -------------------- GUI END ------------------------- */

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("shaders/terrainsplat.vert", "shaders/terrainsplat.frag");
	glUseProgram(program);
	printError("init shader");

	// Load textures
	LoadTGATextureSimple("textures/maskros512.tga", &tex1);
	LoadTGATextureSimple("textures/grass.tga", &splat1);
	LoadTGATextureSimple("textures/dirt.tga", &splat2);
	LoadTGATextureSimple("textures/conc.tga", &splat3);
	LoadTGATextureSimple("textures/splatmap123.tga", &map);

	// Bind and activate textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, splat1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, splat2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, splat3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, map);

	// Upload texture units on GPU
	glUniform1i(glGetUniformLocation(program, "grass"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "dirt"), 1); // Texture unit 1
	glUniform1i(glGetUniformLocation(program, "conc"), 2); // Texture unit 2
	glUniform1i(glGetUniformLocation(program, "map"), 3); // Texture unit 3

	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "opt"), 1);
	
	// Load terrain model
	tm = terrain->setTerrainModel("terrain/fft-terrain.tga");
	printError("init terrain!");

	// Load bunny model
	// bunnyModel = LoadModel("objects/groundsphere.obj");
	bunnyModel = LoadModel("models/bunny.obj");
	sphereModel = LoadModel("objects/groundsphere.obj");

	// Init GUI
	gui->initTerrainGUI(terrain);
}

#include <random>

// Generates a random float between -magnitude and magnitude
float randomOffset(float magnitude) {
    static std::random_device rd; // Obtain a random number from hardware
    static std::mt19937 eng(rd()); // Seed the generator
    std::uniform_real_distribution<> distr(-magnitude, magnitude); // Define the range

    return distr(eng); // Generate and return a random number within the range
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

	// Draw terrain
	tm = terrain->getTerrainModel();
	PlaceModel(tm, program, 0, 0, 0, 0, 0, 0);
	DrawModel(tm, program, "in_Position", "in_Normal", "in_TexCoord");

	/* ------------- GUI ------------------ */
	gui->drawGUI();

	// Define constants for readability and maintenance
	const float DistanceInFrontOfCamera = 15.0f;

	if (GUI::PlaceBunny) {
		GUI::PlaceBunny = false; // Acknowledge the GUI action

		auto cameraPos = camera->getPosition();
		auto forwardVec = camera->getForwardVector();
		// Normalize the forward vector
		forwardVec = Normalize(forwardVec);
		float x = cameraPos.x + DistanceInFrontOfCamera * forwardVec.x;
		float z = cameraPos.z + DistanceInFrontOfCamera * forwardVec.z;
		float y = terrain->getHeightAtPoint(x, z) + 0.6f; // Ensure bunny is placed on the terrain

		GameObject bunny(bunnyModel, x, y, z);
		gameObjects.push_back(bunny);

		// NOTE: Rotation isnt working at all!
		// Calculate the angle (in radians) to make the bunny face towards the camera
		float angleY = atan2(forwardVec.x, forwardVec.z);
		// Convert to degrees
		angleY = angleY * 180 / PI;

		// Debugging prints
		printf("Camera position: (%f, %f, %f)\n", cameraPos.x, cameraPos.y, cameraPos.z);
		printf("Forward vector: (%f, %f, %f)\n", forwardVec.x, forwardVec.y, forwardVec.z);
		printf("Placing bunny at: (%f, %f, %f)\n", x, y, z);
		printf("AngleY: %f\n", angleY);

		// Place the bunny model facing the camera
		PlaceModel(bunnyModel, program, x, y, z, 0, angleY, 0);
	}

	/* ------------- END of GUI ------------------ */
	mat4 modelToWorld;
	worldToView = lookAtv(camera->getPosition(), camera->getPosition() + camera->getForwardVector(), camera->getUpVector()); // camera position, look at point, up vector
	for (auto& gameObject : gameObjects) {
		// Get the model and position of the game object
		Model* model = gameObject.getModel();

		// Identify if gameObject is the bunny
		if (model == bunnyModel) {
			// Generate small random offsets for x and z
			// float offsetX = randomOffset(0.04f);
			// float offsetZ = randomOffset(0.01f);
			float offsetX = 0.05f;
			float offsetZ = 0.01f;
			// printf("OffsetX: %f, OffsetZ: %f\n", offsetX, offsetZ);
			
			// Update gameObject's position with random offsets
			gameObject.move(offsetX, 0, offsetZ);
		}
		GLfloat x = gameObject.x;
		GLfloat z = gameObject.z;
		GLfloat y = terrain->getHeightAtPoint(x, z) + 0.6f;

		// Update the model matrix
		modelToWorld = T(x, y, z);
		glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
		// Draw the model
		DrawModel(model, program, "in_Position", "in_Normal", "in_TexCoord");
	}
	printError("display 2");
	
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	// glutInitWindowSize (1980, 1020);
	glutInitWindowSize (1080, 720);
	glutCreateWindow ("TSBK07 Project");
	glutDisplayFunc(display);
	// glutPassiveMotionFunc(InputController::handleMouseMotionBridge);
	glutKeyboardFunc(InputController::guiKeyboardBridge);
	//glutMouseFunc(onMouse);
	glutMouseFunc(InputController::guiMouseBridge);
	glutMotionFunc(InputController::guiDragBridge);
	init();
	glutRepeatingTimer(20);
	glutMainLoop();
	exit(0);
}
