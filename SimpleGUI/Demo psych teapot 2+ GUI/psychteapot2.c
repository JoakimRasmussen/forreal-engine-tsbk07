// Ingemar's psychedelic teapot 2+

// Special version for SimpleGUI, with just a single slider to control the blue channel.
// For version 0.4, added upscaling It makes sense here.

// uses framework Cocoa
// uses framework OpenGL
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "MicroGlut.h"
#include "SimpleGUI.h"

mat4 projectionMatrix;
mat4 worldToViewMatrix, modelToWorldMatrix;

Model *m;
// Reference to shader program
GLuint program;
float blue = 0.8;

void init(void)
{
	mat4 mx;
	
	// GL inits
	glClearColor(1.0,1.0,1.0,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_TRUE);

	// Load and compile shader
	program = loadShaders("psych.vert", "psych.frag");
	glUseProgram(program);
	
	// Upload geometry to the GPU:
	m = LoadModelPlus("teapot.obj");
	// End of upload of geometry
	
	projectionMatrix = frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 30.0);
	worldToViewMatrix = lookAt(0, 1, 8, 0,0,0, 0,1,0);
	modelToWorldMatrix = IdentityMatrix();
//	mx = Mult(worldToViewMatrix, modelToWorldMatrix);
	
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	
	sgCreateStaticString(40, 60, "BLUE");
	sgCreateSlider(40, 80, 50, &blue, 0, 1);
	// The default color of the background plate is white which makes it invisible on a white background!
	// So set it to something visible!
	sgSetBackgroundColor(0.5, 0.5, 0.5, 0.5);
	// Let't tweak some other colors.
	sgSetTextColor(0, 0, 1);
	sgSetFillColor(0, 0, 0);
	// Since the GUI is so small, let's scale it up!
	sgSetScale(2);
}

GLfloat a, b = 0.0;

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 rot, trans, total;
	
	GLfloat t = glutGet(GLUT_ELAPSED_TIME) / 100.0;

	trans = T(0, -1, 0); // Move teapot to center it
	total = Mult(modelToWorldMatrix, Mult(trans, Rx(-M_PI/2))); // trans centers teapot, Rx rotates the teapot to a comfortable default
	
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glUniform1f(glGetUniformLocation(program, "t"), t);
	glUniform1f(glGetUniformLocation(program, "blue"), blue);

	DrawModel(m, program, "inPosition", NULL, "inTexCoord");
	
	sgDraw();
	
	glutSwapBuffers();
}


int prevx = 0, prevy = 0;

void mouseUpDown(int button, int state, int x, int y)
{
	if (button == 0)
		if (sgMouse(state, x, y)) return;

	if (state == GLUT_DOWN)
	{
		prevx = x;
		prevy = y;
	}
}

void mouseDragged(int x, int y)
{
	if (sgMouseDrag(x, y)) return;
	
	vec3 p;
	mat4 m;
	
	// This is a simple and IMHO really nice trackball system:
	// You must have two things, the worldToViewMatrix and the modelToWorldMatrix
	// (and the latter is modified).
	
	// Use the movement direction to create an orthogonal rotation axis
	p.y = x - prevx;
	p.x = -(prevy - y);
	p.z = 0;

	// Transform the view coordinate rotation axis to world coordinates!
	mat3 wv3 = mat4tomat3(worldToViewMatrix);
	p = MultMat3Vec3(InvertMat3(wv3), p);

	// Create a rotation around this axis and premultiply it on the model-to-world matrix
	m = ArbRotate(p, sqrt(p.x*p.x + p.y*p.y) / 50.0);
	modelToWorldMatrix = Mult(m, modelToWorldMatrix);
	
	prevx = x;
	prevy = y;
	
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutCreateWindow ("Ingemar's psychedelic teapot with GUI");
	glutDisplayFunc(display); 
	glutMouseFunc(mouseUpDown);
	glutMotionFunc(mouseDragged);
	glutRepeatingTimer(20);
	init ();
	glutMainLoop();
	exit(0);
}
