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

GameMode* gameMode = new GameMode();

void init2(void)
{
	gameMode->init();
	printError("init2");
}

void display2(void)
{
	gameMode->run(0, NULL);
	printError("display2");
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	// glutInitWindowSize (1980, 1020);
	glutInitWindowSize (Utils::windowWidth, Utils::windowHeight);
	glutCreateWindow ("TSBK07 Project");
	// glutDisplayFunc(display);
	printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
	glutDisplayFunc(display2);
	// glutPassiveMotionFunc(InputController::handleMouseMotionBridge);
	glutKeyboardFunc(InputController::guiKeyboardBridge);
	glutMouseFunc(InputController::guiMouseBridge);
	glutMotionFunc(InputController::guiDragBridge);
	glutMouseFunc(InputController::onMouseBridge);
	// init();
	init2();
	glutRepeatingTimer(20);
	glutMainLoop();
	exit(0);
}