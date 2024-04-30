#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <vector>
#include "GL_utilities.h"
#include "Utils.h"
#include "LoadTexture.h"
#include "LittleOBJLoader.h"
#include "Camera.h"
#include "Terrain.h"


class Camera;
class Terrain;

class Billboard
{
public:

    // Constructor
    Billboard(Camera *camera, Terrain *terrain);
    // Destructor
    ~Billboard();

    void renderBillboard();
    void addBillboard(vec3 position);

    GLuint billboardShader;
    GLuint billboardTexture;
    Model* billboardModel;
    Camera* camera;
    Terrain* terrain;

    GLfloat posX;
    GLfloat posY;
    GLfloat posZ;
    mat4 modelToViewMatrix;

    std::vector<vec3> billboardPositions;
    

private:


};

#endif // BILLBOARDS_H