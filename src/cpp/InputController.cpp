
#include "../h/InputController.h"

InputController* InputController::instance = nullptr;

InputController::InputController(Camera* camera, Terrain* terrain, Picking* picker, Billboard* billboard) {
    // Constructor implementation
	this->camera = camera;
	this->terrain = terrain;
	this->picker = picker;
	this->billboard = billboard;
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
	if (glutKeyIsDown('q')) {
		vec3 direction = Ry(speed*0.1) * camera->forwardVector;
		camera->forwardVector = normalize(direction);
	}
	if (glutKeyIsDown('e')) {
		vec3 direction = Ry(-speed*0.1) * camera->forwardVector;
		camera->forwardVector = normalize(direction);
	}
	if (glutKeyIsDown('c'))
    {
        vec3 side = CrossProduct(camera->forwardVector, SetVector(0,1,0));
        mat4 m = ArbRotate(side, 0.05);
        camera->forwardVector = MultMat3Vec3(mat4tomat3(m), camera->forwardVector);
    }
    if (glutKeyIsDown('v'))
    {
        vec3 side = CrossProduct(camera->forwardVector, SetVector(0,1,0));
        mat4 m = ArbRotate(side, -0.05);
        camera->forwardVector = MultMat3Vec3(mat4tomat3(m), camera->forwardVector);
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
	terrain->rayTriangleIntersection(camera->getPosition() + camera->getForwardVector(), ray, picker->intersectionPoint, picker->debugIntersectionVector);
	if (GUI::manualElevation)
	{
		GUI::PlaceBunny = false;
		GUI::editTerrainTexture = false;
		GUI::PlaceBillboard = false;
		GUI::smoothTerrain = false;
		terrain->editTerrainAtIntersectionPoint(picker->intersectionPoint);
	}
	if (GUI::smoothTerrain)
	{
		GUI::PlaceBunny = false;
		GUI::editTerrainTexture = false;
		GUI::PlaceBillboard = false;
		GUI::manualElevation = false;
		terrain->smoothTerrainAtIntersectionPoint(picker->intersectionPoint);
	}
	if (GUI::editTerrainTexture)
	{
		GUI::PlaceBunny = false;
		GUI::PlaceBillboard = false;
		GUI::manualElevation = false;
		GUI::smoothTerrain = false;
		terrain->editTerrainTextureAtIntersectionPoint(picker->intersectionPoint, GUI::textureColor, 5);
	}
	if (GUI::PlaceBillboard)
	{
		GUI::manualElevation = false;
		GUI::editTerrainTexture = false;
		GUI::PlaceBunny = false;
		GUI::smoothTerrain = false;
		billboard->addBillboard(picker->intersectionPoint);
	}
	
	if (GUI::PlaceBunny)
	{
		GUI::manualElevation = false;
		GUI::editTerrainTexture = false;
		GUI::PlaceBillboard = false;
		GUI::smoothTerrain = false;
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
		// destroy billboard
		billboard->~Billboard();
		exit(0);
	}
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

void InputController::collectedMouseControllerBridge(int button, int state, int x, int y) {
	if (instance != nullptr) {
		instance->collectedMouseController(button, state, x, y);
	}
}