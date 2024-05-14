#ifndef TERRAIN_H
#define TERRAIN_H

#include "LoadTGA.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include <vector>
#include <cmath>
#include <string>
#include <fstream>

class Terrain {
public:

    Terrain();
    Model* generateTerrain(TextureData* tex, float currentElevation);
    Model* setTerrainModel(const char* heightmap);
    Model* getTerrainModel();
    TextureData* getTextureData();
    
    vec3 getNormalAtPoint(float x, float z) const;
    float getHeightAtPoint(float x, float z) const;
    bool rayTriangleIntersection(vec3 rayOrigin, vec3 rayDirection, vec3& intersectionPoint, std::vector<vec3>& intersectionVector);
    void editTerrainAtIntersectionPoint(vec3 intersectionPoint);
    void smoothTerrainAtIntersectionPoint(vec3 intersectionPoint);
    void updateTerrain();
    void editTerrainTextureAtIntersectionPoint(vec3 intersectionPoint, GLubyte colorPixel[4], int radius);

    // Could be (should be...?) private with a getter
    float currentElevation = 2.0;
    float previousElevation = 2.0;
    float currentMountainHeight = 1100.0;
    float quadSize = 1.0;
    float previousQuadSize = 1.0;
    float heightIncrement = 10;
    bool test = false;
    bool edit = false;
    GLubyte colorPixel[4];

private:
    TextureData ttex;
    TextureData* tex;
    Model* terrainModel;
};

#endif
