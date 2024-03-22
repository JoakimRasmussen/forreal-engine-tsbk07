// Test program for LittleObjLoader with a modified shader,
// mphong.frag, which uses the .mtl data for material colors
// combined with (simplified) Phong shading.
// Uses a modified teapot model split in four parts.
// By Ingemar Ragnemalm 2019-2021

// Modified to a SimpleGUI demo, moving the lid with sliders.

//uses framework Cocoa
//uses framework OpenGL
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "LittleOBJLoader.h"
#include "MicroGlut.h"
#include "SimpleGUI.h"

mat4 projectionMatrix;

// Pointer to model data
Model **teapot;

// Reference to shader program
GLuint program;
vec3 positions = {0, 2, 0};

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 300.0);

	// Load and compile shader
	program = loadShaders("phong.vert", "mphong.frag");
	glUseProgram(program);
	printError("init shader");
	
	// Upload geometry to the GPU:
	teapot = LoadModelSet("teapot-m.obj");
	CenterModel(teapot[3]);
	ReloadModelData(teapot[3]);

	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	vec3 color = SetVector(1, 1, 0);
	glUniform3f(glGetUniformLocation(program, "color"), color.x, color.y, color.z);

	sgCreateSliderVec3Group(40, 40, 100, (float *)&positions, 0, 10);

	printError("init arrays");
}

void mouse(int button, int state, int x, int y)
{
	if (button == 0) sgMouse(state, x, y);
}

void drag(int x, int y)
{
	sgMouseDrag(x, y);
}

GLfloat a = 0.0;

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Matrix4D worldToView, m2, m;
	worldToView = lookAt(0,10,15, 0,4,0, 0,1,0); // Or this - looks higher
	
	a += 0.02;
	m2 = Ry(a);
	m = Mult(worldToView, m2);
	
	glUseProgram(program);
	for (int i = 0; teapot[i] != NULL; i++)
	{
		// Get the part color
		vec3 color = SetVector(1, 1, 1);
		if (teapot[i]->material != NULL)
			color = teapot[i]->material->Kd;
		else
			color = SetVector(((i+6) * 3) % 10 / 10.0, ((i+2) * 4) % 10 / 10.0, (i * 7) % 10 / 10.0);

		m = Mult(worldToView, m2);
		
		// Move the lid
		if (i == 3)
			m = Mult(m, T(positions.x, positions.y, positions.z));
		glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_TRUE, m.m);
		glUniform3f(glGetUniformLocation(program, "color"), color.x, color.y, color.z);
		DrawModel(teapot[i], program, "inPosition", "inNormal", NULL);
	}
	
	sgDraw();
	
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("Put the lid on the teapot");
	glutRepeatingTimer(20);
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	init ();
	glutMainLoop();
}
