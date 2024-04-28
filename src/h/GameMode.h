#ifndef GAME_MODE_H
#define GAME_MODE_H

#include <vector>
#include "Camera.h"
#include "InputController.h"
#include "Terrain.h"
#include "GUI.h"
#include "Utils.h"
#include "GameObject.h"
#include "Picking.h"
#include "GL_utilities.h"
#include "LoadTexture.h"
#include "Billboards.h"

/**
 * Class GameMode handles the main operations for the game including initialization,
 * running game loops, and managing user interactions.
 */
class GameMode {
public:
    // Constructor
    GameMode();

    // Initialize game components
    void init();

    // Main game loop execution
    void run(int argc, char** argv);

    // Elevation button controlled manually
    void manualElevationButton();

private:
    // Helper functions for setting up and managing game frames
    void setupFrameTiming();
    void clearScreen();
    void activateShader(GLuint& shaderProgram);
    void uploadUniforms(GLuint& shaderProgram, const std::string& mode);
    void initGL();
    void loadNecessaryShaders();
    void loadAndBindTextures();
    void loadModels();
    void uploadTextureData(GLuint& shaderProgram, const std::string& mode);
    void setupGUI();


    // Render functions for different game components
    void renderTerrain(GLuint& shaderProgram, Model* tm);
    void renderGUI();
    void renderGameObjects(GLuint& shaderProgram);
    void finalizeFrame();

    // Object interaction functions
    void spawnBunnyOnTerrainClick();
    void updateCameraVariables();
    void updatePositions();
    void uploadPositionData(GLuint& shaderProgram);

    // State variables for time management
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // Essential game components
    Camera* camera;
    Terrain* terrain;
    InputController* inputController;
    GUI* gui;
    Utils* utils;
    Picking* picker;
    Billboards* billboards;

    // Collections for game objects
    std::vector<GameObject> gameObjects;
    std::vector<vec3> objectPositions;

    // Object placement specifics
    const float placementDistance = 10.0f;
    vec3 clickedPosition = vec3(0.0f, 0.0f, 0.0f);

    // Shader programs
    GLuint program, objectShader, billboardShader; // done

    // Texture data and models
    GLuint grass, rock, dirt, map, plant; // done
    GLuint tex1, tex2, furTex, debugTex;
    Model* tm, *bunnyModel, *bill; // done

    // Matrix transformations and vectors for camera and objects
    mat4 worldToView, modelToWorld, lookAtVectors;
    vec3 cameraPos, forwardVec, upVec;

    // Projection matrix reference
    const GLfloat* projectionMatrix;
};

#endif // GAME_MODE_H
