// Demo that shows how you can make an integer slider, going in
// integer steps, just by a single line of "round" on the
// variable controlled by the slider.

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "SimpleGUI.h"
#include <string.h>
#include <math.h>
//uses framework Cocoa
//uses framework OpenGL

float testf;
int testi;

void init()
{
	// A slider and a float display
	sgCreateStaticString(40, 80, "Slider and integer display");
	sgCreateSlider(40, 100, 200, &testf, 5, 25);
	sgCreateDisplayFloat(40, 120, "Value: ", &testf);
	sgCreateDisplayInt(40, 140, "Integer value: ", &testi);
	sgCreateStaticString(40, 180, "Shows how you can make an integer slider");
	sgCreateStaticString(40, 200, "by rounding the controlled variable.");
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	testf = round(testf); // Turns the slider into an integer slider!
						// Skip this line to make the slider continuous
	testi = round(testf); // Just to display it as integer/get an integer value

	sgDraw();
	
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
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
	glutInitWindowSize(600,480);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SimpleGUI integer slider demo");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	
	init();

	glutMainLoop();
}
