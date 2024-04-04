#include "../h/GameMode.h"
#include "GL_utilities.h"

GameMode::GameMode() {
    // Constructor implementation

    // Initial camera settings
    vec3 cameraPosition = vec3(0.0f, 5.0f, 8.0f);
    vec3 forwardVector = vec3(2.0f, 0.0f, 2.0f);
    vec3 cameraUpVector = vec3(0, 1, 0);
    GLfloat cameraSpeed = 0.01f;
    // Camera object
    camera = new Camera(cameraPosition, forwardVector, cameraUpVector, cameraSpeed);
    // Terrain object
    terrain = new Terrain();
    // Input controller object
    inputController = new InputController(camera, terrain);
    // Gui object
    gui = new GUI();
    // Utils object
    // utils = new Utils();
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

	printf("Loading normal shader...\n");
	// Load shader
	program = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	printError("init normal shader");

	printf("Loading textures...\n");
	// Load textures
	LoadTGATextureSimple("textures/maskros512.tga", &tex1);
	LoadTGATextureSimple("textures/rock.tga", &tex2);
	printError("init textures");

	printf("Binding textures...\n");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);

	glUniform1i(glGetUniformLocation(program, "maskrosTexture"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "rockTexture"), 1); // Texture unit 1
	printError("bind textures");

	printf("Loading object shader...\n");
	// Load object shader
	objectShader = loadShaders("shaders/object.vert", "shaders/object.frag");
	glUseProgram(objectShader);
	glUniformMatrix4fv(glGetUniformLocation(objectShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	printError("init shader");

	printf("Loading textures...\n");
	// Load textures
	LoadTGATextureSimple("textures/fur.tga", &furTex);
	printError("init textures");

	printf("Binding textures...\n");
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, furTex);

	// Upload the texture unit for fur texture in object shader
	glUniform1i(glGetUniformLocation(objectShader, "furTex"), 2); // Texture unit 2
	printError("bind textures");

	glUseProgram(program); // Switch back to the terrain shader program
	
	printf("Loading terrain model...\n");
	tm = terrain->setTerrainModel("terrain/fft-terrain.tga");
	printError("init terrain!");

	printf("Loading bunny model...\n");
	bunnyModel = LoadModel("models/bunny.obj");
	printError("init bunny!");

	printf("Initializing TerrainGUI...\n");
	gui->initTerrainGUI(terrain);
	printError("init GUI!");
}

void GameMode::run(int argc, char** argv) {
    // Main game loop
	// Update time based frames
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
	DrawModel(tm, program, "in_Position", "in_Normal", "in_TexCoord");

	/* ------------- GUI ------------- */
	// Draw GUI
	gui->drawGUI();

	// Check if the bunny button is pressed
	bunnyButtonLogic();
	/* ------------- End of GUI -------------*/

	// Draw game objects
	glUseProgram(objectShader);
	glUniformMatrix4fv(glGetUniformLocation(objectShader, "worldToView"), 1, GL_TRUE, worldToView.m);
	glUniform3f(glGetUniformLocation(objectShader, "cameraPosition"), cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform3f(glGetUniformLocation(objectShader, "lightPosition"), 5, 5, 5);
	printError("pre display 2");
	// Rebind and activate the fur texture
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, furTex);
	drawGameObjects();

	glutSwapBuffers();

	printError("new display!");
}

void GameMode::updateCameraVariables() {
	// Update camera position
	cameraPos = camera->getPosition();
	// Update forward vector (normalized)
	forwardVec = Normalize(camera->getForwardVector());
	// Update up vector
	upVec = camera->getUpVector();
}

void GameMode::bunnyButtonLogic() {
	// Test bunny button
	if (GUI::PlaceBunny) {
		GUI::PlaceBunny = false; // Acknowledge the GUI action

		// Place bunny in front of camera
		// Calculate position
		float x = cameraPos.x + placementDistance * forwardVec.x;
		float z = cameraPos.z + placementDistance * forwardVec.z;
		float y = terrain->getHeightAtPoint(x, z) + 0.6f;

		// Create bunny object and add to game objects
		GameObject bunny(bunnyModel, x, y, z);
		gameObjects.push_back(bunny);

		// Place bunny on terrain
		PlaceModel(bunnyModel, program, x, y, z, 0, 0, 0);
	}
}

void GameMode::drawGameObjects() {
	// Draw all game objects
	for (auto& gameObject : gameObjects) {
		
		// Identify the model
		Model* model = gameObject.getModel();
		
		if (model == bunnyModel) {
			// bunny logic
			// move, rotate, etc.
		}
		// Note: introduce getters and setters for x, y, z in GameObject
		GLfloat x = gameObject.x;
		GLfloat z = gameObject.z;
		GLfloat y = terrain->getHeightAtPoint(x, z) + 0.6f;

		// Update the model-to-world matrix
		modelToWorld = T(x, y, z);
		// NOTE: The bunnys position is reseted to the same point for all bunnies if we use
		//       the objectShader. Why is that?
		// glUseProgram(objectShader);
		// glActiveTexture(GL_TEXTURE2);
		// glBindTexture(GL_TEXTURE_2D, furTex);
		// Can optimize to not upload the matrix every time if the objects share textures
		glUniformMatrix4fv(glGetUniformLocation(objectShader, "modelToWorld"), 1, GL_TRUE, modelToWorld.m);
		printError("draw game objects!");
		// Draw the model
		DrawModel(model, program, "in_Position", "in_Normal", "in_TexCoord");
	}
}