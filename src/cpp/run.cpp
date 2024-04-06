// uses framework Cocoa
// uses framework OpenGL
#define MAIN
#include "../h/GameMode.h"

GameMode* gameMode = new GameMode();

void init(void)
{
	gameMode->init();
	printError("init");
}

void display(void)
{
	gameMode->run(0, NULL);
	printError("display");
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (Utils::windowWidth, Utils::windowHeight);
	glutCreateWindow ("TSBK07 Project");
	printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
	glutDisplayFunc(display);

	//glutPassiveMotionFunc(InputController::handleMouseMotionBridge);
	glutKeyboardFunc(InputController::guiKeyboardBridge);
	glutMouseFunc(InputController::guiMouseBridge);
	glutMotionFunc(InputController::guiDragBridge);
	glutMouseFunc(InputController::onMouseBridge);
	init();
	glutRepeatingTimer(20);
	glutMainLoop();
	exit(0);
}