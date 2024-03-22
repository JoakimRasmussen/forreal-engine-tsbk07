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

float testr, testg, testb, testf = 5;
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

float test = 100;

void initGUI(void)
{
	sgCreateStaticString(400, 160, "This is a demo of SimpleGUI");
	// A slider and a float display

	sgCreateStaticString(40, 220, "Slider color group");
	sgCreateSliderColorGroup(20, 240, 150, &testr, &testg, &testb);

	// Radio group: Connect to same variable!
	// sgCreateStaticString(40, 400, "Single line editor:");
	// sgCreateDynamicString(40, 420, testString);
	
	/* sgCreateColorPalette(40, 340, &testr, &testg, &testb);
	sgCreateStaticString(150, 330, "Palette");
	sgCreateButton(150, 350, "RESET", HitButton); */

	/* sgCreateMenu(40, 300, "Colors menu");
	sgCreateMenuItem("Set to red", SetToRed);
	sgCreateMenuItem("Set to green", SetToGreen); 
	sgCreateMenuItem("Set to blue", SetToBlue); */

	// Create a color clicker in the middle of the screen in black
	myCreateGrayScaledColorPalette(40, 500, &testr, &testg, &testb);

	sgSetPosition(70, 20);
}

void keyboard(unsigned char key, int x, int y)
{
	// The "single line editor", edit the string
	int l = strlen(testString);
	if ((key == 127) && (l > 0))
	{
		testString[l-1] = 0;
	}
	else
	if (key >= 32 && l < 128)
	{
		testString[l] = key;
		testString[l+1] = 0;
	}
	
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (button == 0) sgMouse(state, x, y);
	glutPostRedisplay();
}

void drag(int x, int y)
{
	sgMouseDrag(x, y);
	glutPostRedisplay();
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

	// Init GUI
	initGUI();

}

void updateSplat(void)
{
	// Update splat map
	glActiveTexture(GL_TEXTURE3);
	LoadTGATextureSimple("splatmap123.tga", &map);
	glBindTexture(GL_TEXTURE_2D, map);
	glUniform1i(glGetUniformLocation(program, "map"), 3); // Texture unit 3
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
	// Update splat map
	// updateSplat();
	
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
	PlaceModel(tm, program, 0, 0, 0, 0, 0, 0);
	DrawModel(tm, program, "in_Position", "in_Normal", "in_TexCoord");


	/* ------------- GUI ------------------ */
	sgDraw();
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
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	init();
	glutRepeatingTimer(20);
	glutMainLoop();
	exit(0);
}
