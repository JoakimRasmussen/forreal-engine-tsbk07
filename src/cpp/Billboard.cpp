#include "../h/Billboard.h"


// Constructor
Billboard::Billboard(Camera *camera, Terrain *terrain) {
    this->camera = camera;
    this->terrain = terrain;
    this->modelToViewMatrix = IdentityMatrix();
}

// Destructor
Billboard::~Billboard() {
    // Delete the billboard model
    billboardPositions.clear();
    delete this->billboardModel;
}

void Billboard::renderBillboard()
{
    for (unsigned int i = 0; i < this->billboardPositions.size(); i++)
    {
        this->posX = this->billboardPositions[i].x;
        this->posZ = this->billboardPositions[i].z;
        this->posY = terrain->getHeightAtPoint(this->posX, this->posZ);

        // Retrieve the normal vector from the terrain at the clicked position
        vec3 normal = terrain->getNormalAtPoint(posX, posY);

        // Double world orientation
        glUseProgram(this->billboardShader);
        vec3 cameraPos = this->camera->getPosition();
        vec3 forwardVec = this->camera->getForwardVector();
        vec3 upVec = this->camera->getUpVector();
        mat4 worldToView = lookAtv(cameraPos, cameraPos + forwardVec, upVec);

        // Calculate rotation to align with terrain
        float rx = atan2(normal.z, normal.y);
        // If we want camera orientated billboards
        //vec3 toCamera = normalize(cameraPos - vec3(this->posX, this->posY, this->posZ));
        //float ry = atan2(toCamera.x, toCamera.z); // Face camera
        float ry = 0.0f;
        float rz = 0.0f;

        // Camera orientated billboards
        /* // Place first billboard
        this->modelToViewMatrix = worldToView * (T(this->posX, this->posY + 0.5, this->posZ) * Rx(rx) * Ry(ry) * Rz(rz));
        glUniformMatrix4fv(glGetUniformLocation(this->billboardShader, "modelToViewMatrix"), 1, GL_TRUE, this->modelToViewMatrix.m);
        DrawModel(this->billboardModel, this->billboardShader, "in_Position", NULL, "in_TexCoord");
        // Place second billboard with 180 degree rotation
        this->modelToViewMatrix = worldToView * (T(this->posX, this->posY + 0.5, this->posZ) * Rx(rx) * Ry(ry + M_PI) * Rz(rz));
        glUniformMatrix4fv(glGetUniformLocation(this->billboardShader, "modelToViewMatrix"), 1, GL_TRUE, this->modelToViewMatrix.m);
        DrawModel(this->billboardModel, this->billboardShader, "in_Position", NULL, "in_TexCoord");  */

        // World-orientated billboards
        this->modelToViewMatrix = worldToView*(T(this->posX, this->posY, this->posZ)*Rx(rx)*Ry(ry)*Rz(rz)); 
        glUniformMatrix4fv(glGetUniformLocation(this->billboardShader, "modelToViewMatrix"), 1, GL_TRUE, this->modelToViewMatrix.m);
        DrawModel(this->billboardModel, this->billboardShader, "in_Position", NULL, "in_TexCoord");

        this->modelToViewMatrix = Mult(this->modelToViewMatrix, Ry(rz + M_PI/2));
        glUniformMatrix4fv(glGetUniformLocation(this->billboardShader, "modelToViewMatrix"), 1, GL_TRUE, this->modelToViewMatrix.m);
        DrawModel(this->billboardModel, this->billboardShader, "in_Position", NULL, "in_TexCoord");

        this->modelToViewMatrix = Mult(this->modelToViewMatrix, Ry(rz + M_PI));
        glUniformMatrix4fv(glGetUniformLocation(this->billboardShader, "modelToViewMatrix"), 1, GL_TRUE, this->modelToViewMatrix.m);
        DrawModel(this->billboardModel, this->billboardShader, "in_Position", NULL, "in_TexCoord");

        this->modelToViewMatrix = Mult(this->modelToViewMatrix, Ry(rz + M_PI*3/2));
        glUniformMatrix4fv(glGetUniformLocation(this->billboardShader, "modelToViewMatrix"), 1, GL_TRUE, this->modelToViewMatrix.m);
        DrawModel(this->billboardModel, this->billboardShader, "in_Position", NULL, "in_TexCoord");

    }
}

void Billboard::addBillboard(vec3 position)
{
    billboardPositions.push_back(position);
}
