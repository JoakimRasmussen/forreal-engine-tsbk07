#ifndef TERRAIN_H
#define TERRAIN_H

#include "LoadTGA.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"

class Terrain {
public:

    Terrain();
    Model* generateTerrain(TextureData* tex);
    Model* setTerrainModel(const char* heightmap);
    float getHeightAtPoint(float x, float z) const;
    void createSplatMap();


private:
    TextureData ttex;
    TextureData* tex;
    Model* terrainModel;
    float quadSize = 1.0;
};

#endif
