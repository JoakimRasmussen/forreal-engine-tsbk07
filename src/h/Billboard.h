#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <vector>
#include "GL_utilities.h"
#include "Utils.h"
#include "LoadTexture.h"
#include "LittleOBJLoader.h"
#include "Camera.h"


class Camera;

class Billboard
{
public:

    Billboard(Camera *camera);
    void renderBillboard();
    void addBillboard(vec3 position);

    GLuint billboardShader;
    GLuint billboardTexture;
    Model* billboardModel;
    Camera* camera;

    GLfloat posX;
    GLfloat posY;
    GLfloat posZ;
    mat4 modelToViewMatrix;

    std::vector<vec3> billboardPositions;
    

private:


};

#endif // BILLBOARDS_H