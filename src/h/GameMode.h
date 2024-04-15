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

class GameMode {
public:
    GameMode();
    void init();
    void run(int argc, char** argv);
    void updateCameraVariables();
    void spawnBunnyOnTerrainClick();
    void drawGameObjects();
    void manualElevationButton();

    void updatePositions();

private:

    // Time based frames
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // Initial classes
    Camera* camera;
    Terrain* terrain;
    InputController* inputController;
    GUI* gui;
    Utils* utils;
    Picking* picker;

    // Place to store game objects
    std::vector<GameObject> gameObjects;
    std::vector<vec3> objectPositions;

    // Object variables
    const float placementDistance = 10.0f;
    vec3 clickedPosition = vec3(0.0f, 0.0f, 0.0f);

    // Reference to shader program
    GLuint program, objectShader;

    // Texture data
    GLuint splat1, splat2, splat3, map;
    GLuint tex1, tex2, furTex;
    Model *tm, *bunnyModel;

    mat4 worldToView, modelToWorld, lookAtVectors;
    vec3 cameraPos, forwardVec, upVec;

    const GLfloat* projectionMatrix;

    void setupFrameTiming();
    void clearScreen();
    void setupShaders(GLuint& shaderProgram);
    void uploadUniforms(GLuint& shaderProgram, std::string mode);
    void uploadPositionData(GLuint& shaderProgram);
    void renderTerrain(GLuint& shaderProgram, Model* tm);
    void renderGUI();
    void renderGameObjects(GLuint& shaderProgram);
    void finalizeFrame();
};

#endif
