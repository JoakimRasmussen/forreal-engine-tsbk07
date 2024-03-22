// SimpleGUI demo, demonstrating most SimpleGUI controls.

#include "MicroGlut.h"
#include "SimpleGUI.h"
#include <string.h>
//uses framework Cocoa
//uses framework OpenGL

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

void init()
{
	sgCreateStaticString(400, 160, "This is a demo of SimpleGUI");
	
	// A slider and a float display
	
	//sgCreateSmallColorPalette(40, 320, &testr, &testg, &testb);
	sgCreateColorPalette(40, 340, &testr, &testg, &testb);
	sgCreateStaticString(150, 330, "Palette");
	sgCreateButton(150, 350, "RESET", HitButton);

	sgCreateMenu(40, 300, "Colors menu");
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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(40,50);
	glutInitWindowSize(1920,1080);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SimpleGUI demo");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	
	init();

	glutMainLoop();
}
