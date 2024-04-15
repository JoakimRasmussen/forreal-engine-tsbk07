#include "../h/GameMode.h"

GameMode::GameMode() {
    // Constructor implementation

    // Initial camera settings
    vec3 cameraPosition = vec3(0.0f, 5.0f, 8.0f);
    vec3 forwardVector = vec3(2.0f, 0.0f, 2.0f);
    vec3 cameraUpVector = vec3(0, 1, 0);
    GLfloat cameraSpeed = 0.01f;

    objectPositions.reserve(100);

    // Camera object
    camera = new Camera(cameraPosition, forwardVector, cameraUpVector, cameraSpeed);
    // Terrain object
    terrain = new Terrain();
	// Picking object
	picker = new Picking(camera);
    // Input controller object
    inputController = new InputController(camera, terrain, picker);
    // Gui object
    gui = new GUI();

    // Projection matrix
    projectionMatrix = Utils::getProjectionMatrix();

}

void GameMode::init() { 
	// Initialization logic
	printf("Initializing game mode...\n");
    // GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	printf("Loading shaders...\n");
	// Load and compile shader
	program = loadShaders("shaders/terrainsplat.vert", "shaders/terrainsplat.frag");
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	printError("init normal shader");

	printf("Loading textures...\n");
	LoadTGATextureSimple("textures/grass.tga", &splat1);
	LoadTGATextureSimple("textures/dirt.tga", &splat2);
	LoadTGATextureSimple("textures/conc.tga", &splat3);
	LoadTGATextureSimple("splatmap123.tga", &map);
	LoadTGATextureSimple("textures/fur.tga", &furTex);

	printError("init textures");

	printf("Binding textures...\n");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, splat1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, splat2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, splat3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, map);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, furTex);

	printError("bind textures");

	printf("Loading object shader...\n");
	// Load object shader
	objectShader = loadShaders("shaders/object.vert", "shaders/object.frag");
	glUseProgram(objectShader);
	glUniformMatrix4fv(glGetUniformLocation(objectShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	printError("init shader");

	// Upload the texture unit for fur texture in object shader
	glUniform1i(glGetUniformLocation(objectShader, "furTex"), 4); // Texture unit 4
	printError("bind textures");

	glUseProgram(program); // Switch back to the terrain shader program
	printf("Uploading texture units on GPU...\n");
	// Upload texture units on GPU
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	// glUniform1i(glGetUniformLocation(program, "opt"), 1);
	glUniform1i(glGetUniformLocation(program, "grass"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "dirt"), 1); // Texture unit 1
	glUniform1i(glGetUniformLocation(program, "conc"), 2); // Texture unit 2
	glUniform1i(glGetUniformLocation(program, "map"), 3); // Texture unit 3
	
	printf("Loading terrain model...\n");
	tm = terrain->setTerrainModel("terrain/fft-terrain.tga");
	printError("init terrain!");

	printf("Loading bunny model...\n");
	bunnyModel = LoadModel("models/bunny.obj");
	printError("init bunny!");

	printf("Initializing TerrainGUI...\n");
	gui->initTerrainGUI(terrain);
	printError("init GUI!");

	printf("Done initializing game mode\n");
	printf("--------------------------------\n");
}

void GameMode::run(int argc, char** argv) {
    // Main game loop
	// Update time based frames
	printError("pre run");
	GLfloat currentFrame = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	inputController->handleKeyboardInput(deltaTime);

	// Update height of terrain
	terrain->updateTerrain();

	glUseProgram(program);

	// Update camera variables
	updateCameraVariables();

	// Upload world-to-view matrix
	worldToView = lookAtv(cameraPos, cameraPos + forwardVec, upVec); // camera position, look at point, up vector
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE, worldToView.m);

	// Upload camera position
	glUniform3f(glGetUniformLocation(program, "cameraPosition"), cameraPos.x, cameraPos.y, cameraPos.z);

	// Upload light sources
	glUniform3f(glGetUniformLocation(program, "lightPosition"), 5, 5, 5);
	// Send mountain height
	glUniform1f(glGetUniformLocation(program, "mountainHeight"), terrain->currentMountainHeight);
	printError("pre display 1");

	// Draw terrain
	tm = terrain->getTerrainModel();
	PlaceModel(tm, program, 0, 0, 0, 0, 0, 0);

	// --- Shadow ---
	// Update positions of game objects
	updatePositions();
	// Upload object positions to shader
	glUniform3fv(glGetUniformLocation(program, "objectPositions"), objectPositions.size(), &objectPositions[0].x);
	glUniform1i(glGetUniformLocation(program, "numObjects"), objectPositions.size());
	// glUniform3fv(glGetUniformLocation(program, "objectPositions"), objectPositions.size(), &objectPositions[0].x);

	DrawModel(tm, program, "in_Position", "in_Normal", "in_TexCoord");

	// Draw GUI
	gui->drawGUI();

	placeGameObjects();

	// Draw game objects
	glUseProgram(objectShader);
	glUniformMatrix4fv(glGetUniformLocation(objectShader, "worldToView"), 1, GL_TRUE, worldToView.m);
	glUniform3f(glGetUniformLocation(objectShader, "cameraPosition"), cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform3f(glGetUniformLocation(objectShader, "lightPosition"), 5, 5, 5);
	printError("pre display 2");
	// Rebind and activate the fur texture
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, furTex);
	drawGameObjects();

	glutSwapBuffers();

	yOffset += 0.05f;

	printError("new display!");
}


void GameMode::placeGameObjects() {

	if (picker->isPicking) {
		// Update picker state
		picker->updateIsPicking(false);

		// Retrieve the position where the user clicked
		clickedPosition = picker->getIntersectionPoint();

		// Coordinates of the clicked position
		float x = clickedPosition.x;
		float z = clickedPosition.z;

		// Determine y position on terrain, adding a small offset to position the bunny slightly above the terrain
		float y = terrain->getHeightAtPoint(x, z) + 0.6f;

		// Retrieve the normal vector from the terrain at the clicked position
		vec3 normal = terrain->getNormalAtPoint(x, z);

		// Calculate the vector from the bunny position to the camera
		vec3 cameraPos = camera->getPosition();
		vec3 toCamera = Normalize(cameraPos - clickedPosition);

		// Calculate rotation to align with terrain and face camera
		float rx = atan2(normal.z, normal.y);
		float ry = atan2(toCamera.x, toCamera.z);
		float rz = 0.0f;

		// Create a new bunny object with the calculated position and rotations
		GameObject bunny(bunnyModel, x, y, z, rx, ry, rz);
		
		// Add the new bunny to the game objects list
		gameObjects.push_back(bunny);

		// Place the bunny model into the scene with the specified shader
		PlaceModel(bunnyModel, objectShader, x, y, z, rx, ry, rz);
		
		// Check for any errors during the placement operation
		printError("place game objects!");
	}

}
void GameMode::drawGameObjects() {
	// Draw all game objects
	vec3 cameraPos = camera->getPosition();
	for (auto& gameObject : gameObjects) {
		
		// Identify the model
		Model* model = gameObject.getModel();
		
		if (model == bunnyModel) {
			// bunny logic
			// move, rotate, etc.
			gameObject.moveTowardsDestination();
		}
		vec3 objPos = gameObject.getPosition();
		float y = terrain->getHeightAtPoint(objPos.x, objPos.z) + 0.6f;
		objPos.y = y;

		// Update rotations
		vec3 normal = terrain->getNormalAtPoint(objPos.x, objPos.z);
		vec3 toCamera = Normalize(cameraPos - objPos);

		// Calculate rotation to align with terrain and face camera
		gameObject.updateAlignment(normal, toCamera);

		// Retrieve the rotations
		vec3 objRot = gameObject.getRotation();

		// Update the model-to-world matrix
		modelToWorld = T(objPos.x, objPos.y, objPos.z) * Rx(objRot.x) * Ry(objRot.y) * Rz(objRot.z);

		glUniformMatrix4fv(glGetUniformLocation(objectShader, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
		printError("draw game objects!");
		// Draw the model
		DrawModel(model, objectShader, "in_Position", "in_Normal", "in_TexCoord");
	}
}

void GameMode::updatePositions() {
    // Clear the previous position data
    objectPositions.clear();

    // Update positions of game objects
    for (auto& gameObject : gameObjects) {
        // Update the position of the game object
        vec3 objPos = gameObject.getPosition();
        float y = terrain->getHeightAtPoint(objPos.x, objPos.z) + 0.6f;
		objPos.y = y;

        // Store the updated position
        objectPositions.push_back(objPos);

		if (objectPositions.size() >= 100) break;
    }
}

void GameMode::manualElevationButton() {
	// Test manual elevation button
	if (GUI::manualElevation) {
		// Set manual elevation
		terrain->editTerrainAtIntersectionPoint(picker->getIntersectionPoint());
	}
}

void GameMode::updateCameraVariables() {
	// Update camera position
	cameraPos = camera->getPosition();
	// Update forward vector (normalized)
	forwardVec = Normalize(camera->getForwardVector());
	// Update up vector
	upVec = camera->getUpVector();
}