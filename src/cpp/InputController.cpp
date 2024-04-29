
#include "../h/InputController.h"

InputController* InputController::instance = nullptr;

InputController::InputController(Camera* camera, Terrain* terrain, Picking* picker) {
    // Constructor implementation
	this->camera = camera;
	this->terrain = terrain;
	this->picker = picker;
	instance = this;
}

/* CHANGE TO GETTERS AND SETTERS */

void InputController::cameraControls(GLfloat deltaTime, Camera* camera) {
	// Specific controlls for camera
	GLfloat speed = camera->speed * deltaTime;
	
	if (glutKeyIsDown('w')) {
		camera->position += speed * camera->forwardVector;
	}
	if (glutKeyIsDown('s')) {
		camera->position -= speed * camera->forwardVector;
	}
	if (glutKeyIsDown('a')) {
		camera->position -= speed * normalize(cross(camera->forwardVector, camera->upVector));
	}
	if (glutKeyIsDown('d')) {
		camera->position += speed * normalize(cross(camera->forwardVector, camera->upVector));
	}
	if (glutKeyIsDown('z')) {
		camera->position += speed * camera->upVector;
	}
	if (glutKeyIsDown('x')) { 
		camera->position -= speed * camera->upVector;
	}
	// Does not work as intended
	if (glutKeyIsDown('q')) {
		vec3 direction = Ry(speed*0.1) * camera->forwardVector;
		camera->forwardVector = normalize(direction);
	}
	// Does not work as intended
	if (glutKeyIsDown('e')) {
		vec3 direction = Ry(-speed*0.1) * camera->forwardVector;
		camera->forwardVector = normalize(direction);
	}
}

void InputController::collectedMouseController(int button, int state, int x, int y)
{

	if (button == 0) sgMouse(state, x, y);
    glutPostRedisplay();

    if(state != GLUT_DOWN) return;

	// Save clicked coordinates
	this->hitx = x;
	this->hity = y;

	// Check ray
	vec3 ray = picker->calculateMouseRay(x, y, Utils::windowWidth, Utils::windowHeight);
	terrain->rayTriangleIntersection(camera->getPosition(), ray, picker->intersectionPoint, picker->debugIntersectionVector);
	if (GUI::manualElevation && !GUI::PlaceBunny && !GUI::editTerrainTexture)
	{
		GUI::PlaceBunny = false;
		terrain->editTerrainAtIntersectionPoint(picker->intersectionPoint);
	}
	if (GUI::editTerrainTexture && !GUI::manualElevation && !GUI::PlaceBunny)
	{
		terrain->editTerrainTextureAtIntersectionPoint(picker->intersectionPoint, GUI::textureColor, 5);
	}
	
	if (GUI::PlaceBunny)
	{
		GUI::manualElevation = false;
		GUI::editTerrainTexture = false;
		picker->updateIsPicking(true);
		GUI::PlaceBunny = false;
	}
	// TODO: fix
	bool debug = true;
	if (debug)
		for (int i = 0; i < 100; i++)
	{
		{
				// Create a vector of points along the ray
				vec3 pos = camera->getPosition() + i*ray;
				picker->debugRayVector.push_back(pos);
		}
	}
}

void InputController::handleKeyboardInput(GLfloat deltaTime) {
    // Implementation of keyboard input handling
	// General controlls
    cameraControls(deltaTime, camera);
	
	// Escape key
	if (glutKeyIsDown(27)) {
		exit(0);
	}
}

/* NOTE: CURRENTLY NOT USED */
void InputController::handleMouseMotion(int x, int y) {
    // Implementation of mouse motion handli
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}
	float xoffset = x - lastX;
	float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top
	lastX = x;
	lastY = y;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera->yaw += xoffset;
	camera->pitch += yoffset;

	// Make sure that pitch doesn't go outside the range
	if (camera->pitch > 89.0f)
	{
		camera->pitch = 89.0f;
	}
	if (camera->pitch < -89.0f)
	{
		camera->pitch = -89.0f;
	}
	vec3 direction;
	direction.x = cos(utils.degToRad(camera->yaw)) * cos(utils.degToRad(camera->pitch));
	direction.y = sin(utils.degToRad(camera->pitch));
	direction.z = sin(utils.degToRad(camera->yaw)) * cos(utils.degToRad(camera->pitch)); 
	camera->forwardVector = normalize(direction);
}

/* NOTE: CURRENTLY NOT USED */
void InputController::guiKeyboard(unsigned char key, int x, int y)
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

void InputController::guiDrag(int x, int y)
{
	sgMouseDrag(x, y);
	glutPostRedisplay();
}

void InputController::guiDragBridge(int x, int y) {
	if (instance != nullptr) {
		instance->guiDrag(x, y);
	}
}

/* NOTE: CURRENTLY NOT USED */
void InputController::handleMouseMotionBridge(int x, int y) {
    if (instance != nullptr) {
        instance->handleMouseMotion(x, y);
    }
}

/* NOTE: CURRENTLY NOT USED */
void InputController::guiKeyboardBridge(unsigned char key, int x, int y) {
	if (instance != nullptr) {
		instance->guiKeyboard(key, x, y);
	}
}

void InputController::collectedMouseControllerBridge(int button, int state, int x, int y) {
	if (instance != nullptr) {
		instance->collectedMouseController(button, state, x, y);
	}
}