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
    // Gui object
    gui = new GUI();
	// Billboards object
	billboard = new Billboard(camera, terrain);
    // Input controller object
    inputController = new InputController(camera, terrain, picker, billboard);
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
	uploadTextureData(billboard->billboardShader, "billboard");
	uploadTextureData(skyboxShader, "skybox");
	setupGUI();
	Utils::generateColors();

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

	// First Pass: Picking
	// activateShader(pickingShader);
	// uploadUniforms(pickingShader, "picking");
	// renderForPicking(pickingShader);
	// performHitTest();
	// clearScreen();

	// Second Pass: Main rendering
	// Skybox rendering
	activateShader(skyboxShader);
	uploadUniforms(skyboxShader, "skybox");
	renderSkybox(skyboxShader);

	// Terrain rendering
	activateShader(program);
	uploadUniforms(program, "terrain");
	renderTerrain(program, tm);

	// Object rendering
	spawnBunnyOnTerrainClick();
	activateShader(objectShader);
	uploadUniforms(objectShader, "object");
	renderGameObjects(objectShader);
	
	// Billboard rendering
	billboard->renderBillboard();

	// Debugging
	if (debugMode)
	{
		renderDebug();
	}

	// GUI rendering
	renderGUI();

	finalizeFrame();
	printError("Post-run checks");
}

void GameMode::renderDebug()
{
	// Render all points in the vector as spheres
	activateShader(objectShader);
	for (const auto& position : picker->debugIntersectionVector) {
		// Render the debug bunny
		PlaceModelScale(bunnyModel, program, position.x, position.y, position.z, 0, 0, 0, 2, 2, 2);
		DrawModel(bunnyModel, program, "in_Position", "in_Normal", "in_TexCoord");
		printf("Intersection position: %f %f %f\n", position.x, position.y, position.z);
	}
	for (const auto& position : picker->debugRayVector) {
		// Render the sphere
		PlaceModelScale(bunnyModel, program, position.x, position.y, position.z, 0, 0, 0, 1, 1, 1);
		DrawModel(bunnyModel, program, "in_Position", "in_Normal", "in_TexCoord");
	}

	vec3 intersectionPoint = picker->getIntersectionPoint();
	PlaceModelScale(bunnyModel, program, intersectionPoint.x, intersectionPoint.y, intersectionPoint.z, 0, 0, 0, 10, 10, 10);
	DrawModel(bunnyModel, program, "in_Position", "in_Normal", "in_TexCoord");

	if (picker->debugIntersectionVector.size() > 0)
	{
		printf("Intersection %ld debug points\n", picker->debugIntersectionVector.size());
	}
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
		float y = 0;

		GameObject2 bunny(bunnyModel, terrain, objectID);
		bunny.setPosition(x, z);
		bunny.setTexture(furTex, 4);

        // // Determine y position on terrain, adding a small offset to position the bunny slightly above the terrain
        // // float y = terrain->getHeightAtPoint(x, z) + 0.6f;
		// float y = 0;

        // // Retrieve the normal vector from the terrain at the clicked position
        // vec3 normal = terrain->getNormalAtPoint(x, z);

        // // Calculate rotation to align with terrain
        // float rx = atan2(normal.z, normal.y);
        // // float ry = atan2(toCamera.x, toCamera.z); // Face camera
		// float ry = 0.0f;
        // float rz = 0.0f;

        // // Create a new bunny object with the calculated position and rotations
		// bool isSleeping = false;
		// int ID = objectID;
		// objectID++;
        // GameObject bunny(bunnyModel, ID, x, y, z, rx, ry, rz, isSleeping);
        
        // Add the new bunny to the game objects list
        gameObjects.push_back(bunny);

        // Place the bunny model into the scene with the specified shader
        PlaceModel(bunnyModel, objectShader, x, y, z);
		printf("Bunny placed at: %f %f %f\n", x, y, z);
        
        // Check for any errors during the placement operation
        printError("Spawn bunny on terrain click!");
    }
}

/* For shadow rendering */
void GameMode::updatePositions() {
    // Clear the previous position data
    objectPositions.clear();

    // Update positions of game objects
    for (auto& gameObject : gameObjects) {
        // Update the position of the game object
        vec3 objPos = gameObject.getPosition();
        // float y = terrain->getHeightAtPoint(objPos.x, objPos.z) + 0.6f;
		// objPos.y += y;

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
	// Universal uniforms
	if (mode != "skybox") {
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldToView"), 1, GL_TRUE, worldToView.m);
	}

	// Mode specific uniforms
	if (mode == "terrain" || mode == "object") {
		glUniform3f(glGetUniformLocation(shaderProgram, "cameraPosition"), cameraPos.x, cameraPos.y, cameraPos.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightPosition"), 5, 5, 5);
	}
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

		gameObject.renderGameObject(shaderProgram, false);
		// // Identify the model
		// Model* model = gameObject.getModel();
		// if (model == bunnyModel) {
		// 	gameObject.bunnyMovement();
		// }
		// vec3 objPos = gameObject.getPosition();
		// float y = terrain->getHeightAtPoint(objPos.x, objPos.z) + 0.6f;
		// objPos.y += y;

		// // Update rotations
		// vec3 normal = terrain->getNormalAtPoint(objPos.x, objPos.z);

		// // Calculate rotation to align with terrain and face camera
		// gameObject.updateAlignmentToTerrain(normal);

		// // Retrieve the rotations
		// vec3 objRot = gameObject.getRotation();

		// // Update the model-to-world matrix
		// modelToWorld = T(objPos.x, objPos.y, objPos.z) * Rx(objRot.x) * Ry(objRot.y) * Rz(objRot.z);

		// activateShader(shaderProgram);
		// glActiveTexture(GL_TEXTURE4);
    	// glBindTexture(GL_TEXTURE_2D, furTex);
		// glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
		// // Draw the model
		// DrawModel(model, shaderProgram, "in_Position", "in_Normal", "in_TexCoord");
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
	billboard->billboardShader = loadShaders("shaders/billboardShader.vert", "shaders/billboardShader.frag");
	pickingShader = loadShaders("shaders/pick.vert", "shaders/pick.frag");
	skyboxShader = loadShaders("shaders/skybox.vert", "shaders/skybox.frag");

	// Upload the projection matrix to the shader programs
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUseProgram(objectShader);
	glUniformMatrix4fv(glGetUniformLocation(objectShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUseProgram(billboard->billboardShader);
	glUniformMatrix4fv(glGetUniformLocation(billboard->billboardShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUseProgram(pickingShader);
	glUniformMatrix4fv(glGetUniformLocation(pickingShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUseProgram(skyboxShader);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

    printError("shader compilation");
}

void GameMode::loadAndBindTextures() {
    printf("Loading and binding textures...\n");

	LoadTGATextureSimple("textures/grass.tga", &grass);
    LoadTGATextureSimple("textures/dirt.tga", &dirt);
    LoadTGATextureSimple("textures/rock.tga", &rock);
    // LoadTGATextureSimple("textures/fur.tga", &furTex);

	// grass = LoadTexture("ue/grass2k/hres_grass.jpg", 1);
	// dirt = LoadTexture("ue/dirt2k/hres_dirt.jpg", 1);
	// rock = LoadTexture("ue/rock2k/hres_rock.jpg", 1);
	furTex = LoadTexture("ue/fur2k/hres_fur.jpg", 1);
	billboard->billboardTexture = LoadTexture("ue/bil-plants/billboard-plant.png", 1);

    LoadTGATextureSimple("splatmap.tga", &map);
    LoadTGATextureSimple("textures/rutor.tga", &debugTex);
	LoadTGATextureSimple("textures/SkyBoxFull.tga", &skyboxTex);

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, debugTex);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, billboard->billboardTexture);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, skyboxTex);

    printError("texture loading and binding");
}

void GameMode::loadModels() {
    printf("Loading models...\n");
    bunnyModel = LoadModel("models/bunnyplus.obj");
    tm = terrain->setTerrainModel("terrain/fft-terrain-test.tga");
    // tm = terrain->setTerrainModel("terrain/fft-terrain.tga");
	billboard->billboardModel = LoadModel("models/bill.obj");
	skyboxModel = LoadModelPlus("models/skyboxfull.obj");
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
	// printf("Using shader program: %d\n", shaderProgram);

	// Shared textures

	// Terrain specific textures
	if (mode == "terrain") {
		printf("Uploading terrain textures...\n");
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shaderProgram, "grass"), 0);
		printError("grass");
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
		glActiveTexture(GL_TEXTURE4);
		glUniform1i(glGetUniformLocation(shaderProgram, "objTex"), 4);

		//glUniform1i(glGetUniformLocation(shaderProgram, "debugTex"), 5);
		printError("upload textures (object)");
	}
	// Billboard specific textures
	else if (mode == "billboard")
	{
		printf("Uploading billboard textures...\n");
		glActiveTexture(GL_TEXTURE6);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 6);
		printError("upload textures (billboard)");
	}	
	// Skybox specific textures
	else if (mode == "skybox")
	{
		printf("Uploading skybox textures...\n");
		glActiveTexture(GL_TEXTURE7);
		glUniform1i(glGetUniformLocation(shaderProgram, "skyboxTex"), 7);
		printError("upload textures (skybox)");
	}
	else
	
	printError("upload textures");
}

void GameMode::renderForPicking(GLuint& pickingShader) {
    if (gameObjects.empty()) {
        return;
    }

    for (std::size_t i = 0; i < gameObjects.size(); i++) {
        int colorIndex = i % numColors;
        if (!colorHits[colorIndex]) {
            // Set the color through Utils
            const GLfloat* color = Utils::getColor(colorIndex);
            glUniform4fv(glGetUniformLocation(pickingShader, "objectColor"), 1, color);
        } else {
            // Option to set to white or delete the object
            deleteObject(gameObjects[i].getObjectID());
        }
        printError("color setting");

		gameObjects[i].renderGameObject(pickingShader, true);

        // // Position and rotation updates remain unchanged
        // vec3 objPos = gameObjects[i].getPosition();
        // float y = terrain->getHeightAtPoint(objPos.x, objPos.z) + 0.6f;
        // objPos.y += y;
        // vec3 objRot = gameObjects[i].getRotation();
        // modelToWorld = T(objPos.x, objPos.y, objPos.z) * Rx(objRot.x) * Ry(objRot.y) * Rz(objRot.z);
        // glUniformMatrix4fv(glGetUniformLocation(pickingShader, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);

        // DrawModel(gameObjects[i].getModel(), pickingShader, "in_Position", NULL, NULL);
        // printError("draw object");
    }
}

void GameMode::performHitTest() {

	int hitx = inputController->getHitX();
	int hity = inputController->getHitY();
	float pickedColor[4];

	glReadPixels(hitx, Utils::windowHeight - hity - 1, 1, 1, GL_RGBA, GL_FLOAT, &pickedColor);
    printError("glReadPixels");
	inputController->resetHitCoordinates();
	
	// Reset the color hits
    for (int i = 0; i < numColors; i++) {
        colorHits[i] = 0;
    }

    // Using the new Utils function to check for color match
    int matchedColorIndex = Utils::colorMatch(pickedColor);
    if (matchedColorIndex != -1) {
        // Increment the hit counter for the corresponding color
        colorHits[matchedColorIndex]++;
    }
}

void GameMode::deleteObject(int objectID) {
    for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it) {
        if (it->getObjectID() == objectID) {
            it = gameObjects.erase(it);
            break;
        }
    }
}

void GameMode::printObjectIDs() {
	printf("Object IDs: ");
	for (auto& gameObject : gameObjects) {
		printf("%d ", gameObject.getObjectID());
	}
	printf("\n");
}

void GameMode::renderSkybox(GLuint& shaderProgram) {
	glEnable(GL_DEPTH_CLAMP);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	activateShader(shaderProgram);
	mat4 modelToWorld = IdentityMatrix();
	mat4 worldToView = lookAtv(SetVec3(0,0,0), forwardVec, upVec);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldToView"), 1, GL_TRUE, worldToView.m);
	// Rebind the skybox texture
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, skyboxTex);
	DrawModel(skyboxModel, shaderProgram, "in_Position", NULL, "in_TexCoord");

	glDisable(GL_DEPTH_CLAMP);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	printError("draw skybox");
}
