// Phong bunny and friends
// This demo shows how you can mix shaders as well as models in the same scene

// SimpleGUI version.

//uses framework Cocoa
//uses framework OpenGL
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "MicroGlut.h"
#include "SimpleGUI.h"

mat4 projectionMatrix;

Model *bunny, *teddy;

// Reference to shader programs
GLuint phongShader, texShader;

// Variables controlled by simpleGUI:
int rotate = 1;
vec3 color = {1, 1, 1};
vec3 angles = {0, 0, 0};

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 300.0);

	// Load and compile shader
	phongShader = loadShaders("phong.vert", "phong.frag");
	texShader = loadShaders("proctex.vert", "proctex.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	bunny = LoadModelPlus("bunnyplus.obj"); // Bunny with texture coords
	teddy = LoadModelPlus("teddy.obj"); // Bunny with texture coords

// Important! The shader we upload to must be active!
	glUseProgram(phongShader);
	glUniformMatrix4fv(glGetUniformLocation(phongShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUseProgram(texShader);
	glUniformMatrix4fv(glGetUniformLocation(texShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	
	printError("init arrays");
	
	sgCreateSliderColorGroup(340, 40, 200, &color.x, &color.y, &color.z);
	sgCreateCheckBox(340, 100, "Rotate", &rotate);
	sgCreateSliderVec3Group(340, 140, 100, (float *)&angles, -1, 1);
}

GLfloat a = 0.0;

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mat4 worldToView, m; // m1, m2, m, tr, scale;
	
	worldToView = lookAt(0,0,4, 0,0,0, 0,1,0);
	
	// Phong bunny
	a += 0.1;
	
	m = Mult(worldToView, Mult(T(-1, 1, 0), Mult(Ry(a), Rz(M_PI/5))));
	
	glUseProgram(phongShader);
	glUniform3f(glGetUniformLocation(phongShader, "color"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(phongShader, "modelviewMatrix"), 1, GL_TRUE, m.m);
	
	DrawModel(bunny, phongShader, "inPosition", "inNormal", NULL);
	
	// Colored bunny
	glUseProgram(texShader);
//	m = Mult(worldToView, Mult(T(-1, -1, 0), Mult(Ry(-a),Rz(M_PI/5))));
	m = Mult(worldToView, Mult(T(-1, -1, 0), Mult(Rx(angles.x), Mult(Ry(angles.y),Rz(angles.z)))));
	glUniformMatrix4fv(glGetUniformLocation(texShader, "modelviewMatrix"), 1, GL_TRUE, m.m);
	DrawModel(bunny, texShader, "inPosition", "inNormal", "inTexCoord");
	
	// Phong Teddy
	if (rotate)
		m = Mult(worldToView, Mult(T(1, sin(a), -2), Rx(3*sin(a/3))));
	else
		m = Mult(worldToView, Mult(T(1, sin(M_PI), -2), Rx(3*sin(M_PI))));
	
	glUseProgram(phongShader);
	glUniform3f(glGetUniformLocation(phongShader, "color"), 1, 1, 1);
	glUniformMatrix4fv(glGetUniformLocation(phongShader, "modelviewMatrix"), 1, GL_TRUE, m.m);
	DrawModel(teddy, phongShader, "inPosition", "inNormal", NULL);
	
	printError("display");
	
	sgDraw();

	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
}

void mouse(int button, int state, int x, int y)
{
	if (button == 0) sgMouse(state, x, y);
}

void drag(int x, int y)
{
	sgMouseDrag(x, y);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutCreateWindow ("GL3 phong bunny with friends + simpleGUI");
	glutRepeatingTimerFunc(20);
	glutDisplayFunc(display); 
	glutKeyboardFunc(keys);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	init ();
	glutMainLoop();
}
