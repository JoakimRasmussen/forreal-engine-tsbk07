// SimpleGUI text demo

#include "MicroGlut.h"
#include "SimpleGUI.h"
#include <string.h>
//uses framework Cocoa
//uses framework OpenGL

void init()
{
	sgCreateStaticString(100, 40, "This is a demo of multi-line text\rusing SimpleGUI");
	
	// A slider and a float display
	sgCreateStaticString(40, 100,\
	"This is a text which spans multiple lines\r"\
	"which can be nice if you want to display a lot of text\r"\
	"or build your own dynamic information to a string\r"\
	"which you can do as much as you like this way."
	);

	sgSetPosition(20, 20);
	sgSetTextLineSpacing(16);

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
	glutCreateWindow("SimpleGUI text demo");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	
	init();

	glutMainLoop();
}
