#include "../h/Billboards.h"


// Constructor
Billboards::Billboards(Camera *camera) {
    this->camera = camera;
    this->billboardPosition = vec3(0.0f, 0.0f, 0.0f);
    this->modelToViewMatrix = IdentityMatrix();
}

void Billboards::renderBillboard()
{
    glUseProgram(this->billboardShader);

    vec3 cameraPos = this->camera->getPosition();
    vec3 forwardVec = this->camera->getForwardVector();
    vec3 upVec = this->camera->getUpVector();
    mat4 worldToView = lookAtv(cameraPos, cameraPos + forwardVec, upVec);
	this->modelToViewMatrix = worldToView*(T(5, 1, 0)*Ry(0)*Rz(M_PI/8)); // T(POSITION)
	glUniformMatrix4fv(glGetUniformLocation(this->billboardShader, "modelToViewMatrix"), 1, GL_TRUE, this->modelToViewMatrix.m);
	DrawModel(this->billboardModel, this->billboardShader, "in_Position", NULL, "in_TexCoord");

	this->modelToViewMatrix = Mult(this->modelToViewMatrix, Ry(3.14/2));
	glUniformMatrix4fv(glGetUniformLocation(this->billboardShader, "modelToViewMatrix"), 1, GL_TRUE, this->modelToViewMatrix.m);
	DrawModel(this->billboardModel, this->billboardShader, "in_Position", NULL, "in_TexCoord");
}
