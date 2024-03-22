// Stepper demo to demonstrate this new control.

#include "MicroGlut.h"
#include "SimpleGUI.h"
#include <string.h>
//uses framework Cocoa
//uses framework OpenGL

int testi = 10;

void init()
{
	sgCreateStaticString(40, 40, "Stepper demo");

//	sgCreateLeftStepper(40, 150, &testi);
//	sgCreateRightStepper(50, 150, &testi);
	
	sgCreateSteppers(40, 80, &testi);
	sgCreateDisplayInt(80, 80, "Value: ", &testi);

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sgDraw();
	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
	sgMouse(state, x, y);
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
	glutInitWindowPosition(80,80);
	glutInitWindowSize(240,100);
	glutInitContextVersion(3, 2);
	glutCreateWindow("Stepper demo");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	
	init();

	glutMainLoop();
}
