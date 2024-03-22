// SimpleGUI auto-placement demo

#include "MicroGlut.h"
#include "SimpleGUI.h"
#include <string.h>
//uses framework Cocoa
//uses framework OpenGL

void init()
{
	sgCreateStaticString(-1, -1, "This is a demo of auto-placement");
	sgCreateStaticString(-1, -1, "If I just want things from top to bottom");
	sgCreateStaticString(-1, -1, "I can pass -1 to get the prevous value");
	sgCreateStaticString(-1, -1, "plus a Y offset.");
	

	sgSetPosition(20, 20);
	sgSetSpacing(16);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	glutInitWindowSize(700,300);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SimpleGUI auto-placement demo");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	
	init();

	glutMainLoop();
}
