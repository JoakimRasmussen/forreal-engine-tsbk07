#include "../h/Billboard.h"


// Constructor
Billboard::Billboard(Camera *camera) {
    this->camera = camera;
    this->modelToViewMatrix = IdentityMatrix();
}

void Billboard::renderBillboard()
{
    for (unsigned int i = 0; i < this->billboardPositions.size(); i++)
    {
        this->posX = this->billboardPositions[i].x;
        this->posY = this->billboardPositions[i].y;
        this->posZ = this->billboardPositions[i].z;
        
        glUseProgram(this->billboardShader);
        vec3 cameraPos = this->camera->getPosition();
        vec3 forwardVec = this->camera->getForwardVector();
        vec3 upVec = this->camera->getUpVector();
        mat4 worldToView = lookAtv(cameraPos, cameraPos + forwardVec, upVec);
        this->modelToViewMatrix = worldToView*(T(this->posX, this->posY, this->posZ)*Ry(0)*Rz(M_PI/8)); // T(POSITION)
        glUniformMatrix4fv(glGetUniformLocation(this->billboardShader, "modelToViewMatrix"), 1, GL_TRUE, this->modelToViewMatrix.m);
        DrawModel(this->billboardModel, this->billboardShader, "in_Position", NULL, "in_TexCoord");

        this->modelToViewMatrix = Mult(this->modelToViewMatrix, Ry(3.14/2));
        glUniformMatrix4fv(glGetUniformLocation(this->billboardShader, "modelToViewMatrix"), 1, GL_TRUE, this->modelToViewMatrix.m);
        DrawModel(this->billboardModel, this->billboardShader, "in_Position", NULL, "in_TexCoord");
    }
}

void Billboard::addBillboard(vec3 position)
{
    billboardPositions.push_back(position);
}
