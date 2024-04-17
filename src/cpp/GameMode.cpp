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
	printError("Pre-init checks");
	printf("Initializing game mode...\n");

	initGL();
	loadNecessaryShaders();
	loadAndBindTextures();
	loadModels();
	uploadTextureData(program, "terrain");
	uploadTextureData(objectShader, "object");
	setupGUI();

	printf("Done initializing game mode\n");
	printError("Post-init checks");
	printf("--------------------------------\n");
}

void GameMode::run(int argc, char** argv) {
    printError("Pre-run checks");
	setupFrameTiming();
	clearScreen();

	inputController->handleKeyboardInput(deltaTime);
	terrain->updateTerrain();
	tm = terrain->getTerrainModel();

	updateCameraVariables();
	updatePositions();

	activateShader(program);
	uploadUniforms(program, "terrain");
	renderTerrain(program, tm);

	spawnBunnyOnTerrainClick();

	activateShader(objectShader);
	uploadUniforms(objectShader, "object");
	renderGameObjects(objectShader);

	renderGUI();

	finalizeFrame();
	printError("Post-run checks");
}


void GameMode::spawnBunnyOnTerrainClick() {
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

        // Calculate rotation to align with terrain
        float rx = atan2(normal.z, normal.y);
        // float ry = atan2(toCamera.x, toCamera.z); // Face camera
		float ry = 0.0f;
        float rz = 0.0f;

        // Create a new bunny object with the calculated position and rotations
        GameObject bunny(bunnyModel, x, y, z, rx, ry, rz);
        
        // Add the new bunny to the game objects list
        gameObjects.push_back(bunny);

        // Place the bunny model into the scene with the specified shader
        PlaceModel(bunnyModel, objectShader, x, y, z, rx, ry, rz);
        
        // Check for any errors during the placement operation
        printError("Spawn bunny on terrain click!");
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
	worldToView = lookAtv(cameraPos, cameraPos + forwardVec, upVec);
}

void GameMode::setupFrameTiming() {
	// Update time based frames
	GLfloat currentFrame = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void GameMode::clearScreen() {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameMode::activateShader(GLuint& shaderProgram) {
	glUseProgram(shaderProgram);
}

void GameMode::uploadUniforms(GLuint& shaderProgram, const std::string& mode) {
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldToView"), 1, GL_TRUE, worldToView.m);
	glUniform3f(glGetUniformLocation(shaderProgram, "cameraPosition"), cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "lightPosition"), 5, 5, 5);
	if (mode == "terrain") {
		glUniform1f(glGetUniformLocation(shaderProgram, "mountainHeight"), terrain->currentMountainHeight);
	}
}

void GameMode::uploadPositionData(GLuint& shaderProgram) {
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectPositions"), objectPositions.size(), &objectPositions[0].x);
	glUniform1i(glGetUniformLocation(shaderProgram, "numObjects"), objectPositions.size());
}

void GameMode::renderTerrain(GLuint& shaderProgram, Model* tm) {
	// Get updated terrain model
	PlaceModel(tm, shaderProgram, 0, 0, 0, 0, 0, 0);

	// Upload object positions to shader for shadows
	uploadPositionData(shaderProgram);

	// Draw terrain
	DrawModel(tm, shaderProgram, "in_Position", "in_Normal", "in_TexCoord");
}

void GameMode::renderGUI() {
	gui->drawGUI();
}

void GameMode::renderGameObjects(GLuint& shaderProgram) {
	// Render game objects

	for (auto& gameObject : gameObjects) {
		// Identify the model
		Model* model = gameObject.getModel();
		gameObject.moveTowardsDestination();
		vec3 objPos = gameObject.getPosition();
		float y = terrain->getHeightAtPoint(objPos.x, objPos.z) + 0.6f;
		objPos.y = y;

		// Update rotations
		vec3 normal = terrain->getNormalAtPoint(objPos.x, objPos.z);

		// Calculate rotation to align with terrain and face camera
		gameObject.updateAlignmentToTerrain(normal);

		// Retrieve the rotations
		vec3 objRot = gameObject.getRotation();

		// Update the model-to-world matrix
		modelToWorld = T(objPos.x, objPos.y, objPos.z) * Rx(objRot.x) * Ry(objRot.y) * Rz(objRot.z);

		activateShader(shaderProgram);
		glActiveTexture(GL_TEXTURE4);
    	glBindTexture(GL_TEXTURE_2D, furTex);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
		// Draw the model
		DrawModel(model, shaderProgram, "in_Position", "in_Normal", "in_TexCoord");
		// DrawModel(model, shaderProgram, "in_Position", NULL, "in_TexCoord");
	}	
}

void GameMode::finalizeFrame() {
	glutSwapBuffers();
}

void GameMode::initGL() {
    printf("Initializing GL settings...\n");
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    printError("GL inits");
}

void GameMode::loadNecessaryShaders() {
    printf("Loading shaders...\n");
    program = loadShaders("shaders/terrainsplat.vert", "shaders/terrainsplat.frag");
    objectShader = loadShaders("shaders/object.vert", "shaders/object.frag");

	// Upload the projection matrix to the shader programs
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUseProgram(objectShader);
	glUniformMatrix4fv(glGetUniformLocation(objectShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

    printError("shader compilation");
}

void GameMode::loadAndBindTextures() {
    printf("Loading and binding textures...\n");
    LoadTGATextureSimple("textures/grass.tga", &grass);
    LoadTGATextureSimple("textures/dirt.tga", &dirt);
    LoadTGATextureSimple("textures/rock.tga", &rock);
    LoadTGATextureSimple("splatmap.tga", &map);
    LoadTGATextureSimple("textures/fur.tga", &furTex);
    LoadTGATextureSimple("textures/rutor.tga", &debugTex);

	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grass);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, dirt);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, rock);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, map);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, furTex);
	glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, debugTex);

    printError("texture loading and binding");
}

void GameMode::loadModels() {
    printf("Loading models...\n");
    bunnyModel = LoadModel("models/bunny.obj");
    tm = terrain->setTerrainModel("terrain/fft-terrain.tga");
    printError("model loading");
}

void GameMode::setupGUI() {
    printf("Setting up GUI...\n");
    gui->initTerrainGUI(terrain);
    printError("GUI setup");
}

void GameMode::uploadTextureData(GLuint& shaderProgram, const std::string& mode) {
	// Activate the shader program
	activateShader(shaderProgram);

	// Shared textures
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaderProgram, "grass"), 0);
	printError("grass");

	// Terrain specific textures
	if (mode == "terrain") {
		printf("Uploading terrain textures...\n");
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(glGetUniformLocation(shaderProgram, "dirt"), 1);
		glActiveTexture(GL_TEXTURE2);
		glUniform1i(glGetUniformLocation(shaderProgram, "rock"), 2);
		glActiveTexture(GL_TEXTURE3);
		glUniform1i(glGetUniformLocation(shaderProgram, "map"), 3);
		printError("upload textures (terrain)");
	}
	// Object specific textures
	else if (mode == "object")
	{
		printf("Uploading object textures...\n");
		glUniform1i(glGetUniformLocation(shaderProgram, "furTex"), 4);
		glUniform1i(glGetUniformLocation(shaderProgram, "debugTex"), 5);
		printError("upload textures (object)");
	}	
	
	printError("upload textures");
}

