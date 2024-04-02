#ifndef TERRAIN_H
#define TERRAIN_H

#include "LoadTGA.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"

class Terrain {
public:

    Terrain();
    Model* generateTerrain(TextureData* tex, float currentElevation);
    Model* setTerrainModel(const char* heightmap);
    float getHeightAtPoint(float x, float z) const;
    void createSplatMap();
    TextureData* getTextureData();
    void updateTerrain();
    Model* getTerrainModel();

    // Could be (should be...?) private with a getter
    float currentElevation = 5.0;
    float previousElevation = 5.0;
    float quadSize = 1.0;
    float currentMountainHeight = 15.0;

private:
    TextureData ttex;
    TextureData* tex;
    Model* terrainModel;
};

#endif
