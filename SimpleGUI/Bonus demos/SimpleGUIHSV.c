// Test with an HSV group.
// I considered making the HSV group built-in but I am not sure it is interesting enough
// and I want to keep SimpleGUI small.

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "SimpleGUI.h"
#include <string.h>
//uses framework Cocoa
//uses framework OpenGL

void HSV2RGB (float h, float s, float v, float *red, float *green, float *blue)
{
	float a, b, c, fraction;
	int i;

	if (s == 0) // grayscale!}
		{
			*red = v;
			*green = v;
			*blue = v;
		}
	else
		{
			h = h * 6;
			i = (int)h; //floor}
			fraction = h - i;
			a = v * (1 - s);
			b = v * (1 - s * fraction);
			c = v * (1 - s * (1 - fraction));
			switch (i)
			{
				case 0: case 6: 
						*red = v;
						*green = c;
						*blue = a;
					break;
				case 1: 
						*red = b;
						*green = v;
						*blue = a;
					break;
				case 2: 
						*red = a;
						*green = v;
						*blue = c;
					break;
				case 3: 
						*red = a;
						*green = b;
						*blue = v;
					break;
				case 4: 
						*red = c;
						*green = a;
						*blue = v;
					break;
				case 5: 
						*red = v;
						*green = a;
						*blue = b;
					break;
			} // case

		}
}


float red, green, blue;
float h, s=1, v=1;

// I consider this as a possible addition to SimpleGUI, but it is that interesting?
// I think it is just fine to have the HSV conversion outside the main library
// in order to keep it small.
void sgCreateHSVGroup(int x, int y, int w, float *red, float *green, float *blue)
{
	sgCreateColorDrop(x + 0, y + 20, red, green, blue);
	sgCreateSlider(x + 30, y + 0, w, &h, 0, 1);
	sgCreateSlider(x + 30, y + 20, w, &s, 0, 1);
	sgCreateSlider(x + 30, y + 40, w, &v, 0, 1);
	
	// When can I then do the conversion?
	// When drawing, if I mark the color drop some way!
}

void init()
{
	sgCreateStaticString(40, 40, "This demo creates three sliders and pass");
	sgCreateStaticString(40, 60, "the values to a HSV to RGB converter");
	sgCreateStaticString(40, 80, "that puts the result in a color drop.");
	sgCreateHSVGroup(80, 100, 200, &red, &green, &blue);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	HSV2RGB (h, s, v, &red, &green, &blue);

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
	glutCreateWindow("SimpleGUI HSV demo");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	
	init();

	glutMainLoop();
}
