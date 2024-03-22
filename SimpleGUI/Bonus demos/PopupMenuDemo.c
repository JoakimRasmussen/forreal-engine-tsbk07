// Menu demo, demonstrating popup menus.
// This is part of the big demo but isolated here for focus.

#include "MicroGlut.h"
#include "SimpleGUI.h"
#include <string.h>
//uses framework Cocoa
//uses framework OpenGL

float testr, testg, testb;

float myvec3[3];

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

void init()
{
	sgCreateStaticString(40, 40, "SimpleGUI popup menu demo");
	
	sgCreateStaticString(40, 70, "Slider color group");
	sgCreateSliderColorGroup(20, 90, 150, &testr, &testg, &testb);

	sgCreateSmallColorPalette(40, 180, &testr, &testg, &testb);

	sgCreateMenu(40, 160, "Colors menu");
	sgCreateMenuItem("Set to red", SetToRed);
	sgCreateMenuItem("Set to green", SetToGreen);
	sgCreateMenuItem("Set to blue", SetToBlue);

	sgSetPosition(70, 20);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sgDraw();
	glutSwapBuffers();
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
	glutInitWindowSize(500,300);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SimpleGUI popup menu demo");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	
	init();

	glutMainLoop();
}
