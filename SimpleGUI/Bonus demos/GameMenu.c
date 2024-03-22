// SimpleGUI game menu demo, with scaling

#include "MicroGlut.h"
#include "SimpleGUI.h"
#include <string.h>
//uses framework Cocoa
//uses framework OpenGL

int testradio;

void HitButton()
{
	printf("Make game start here\n");
}

void init()
{
	sgCreateStaticString(100, 100, "My interesting game menu");
	
	// Radio group: Connect to same variable!
	sgCreateStaticString(100, 140, "Select level:");
	sgCreateRadio(100, 160, "One", &testradio, 1);
	sgCreateRadio(100, 180, "Two", &testradio, 2);
	sgCreateRadio(100, 200, "Three", &testradio, 3);

	sgCreateButton(150, 260, " RUN GAME ", HitButton);
	
	sgSetScale(2);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sgDraw();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(40,50);
	glutInitWindowSize(850,700);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SimpleGUI game menu demo");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	
	init();

	glutMainLoop();
}
