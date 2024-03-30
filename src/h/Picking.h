#ifndef PICKING_H
#define PICKING_H

#include "VectorUtils4.h"
#include "../h/Camera.h"

class Picking {

    public:

        // Constructor
        Picking(Camera* camera);
        // Destructor
        ~Picking();

        // Methods
        void update();
        // Ray casting
        vec3 calculateMouseRay(float mouseX, float mouseY, int screenWidth, int screenHeight);
        vec3 normalizeScreenCoordinates(float mouseX, float mouseY, int screenWidth, int screenHeight);
        vec4 toViewCoordinates(vec4 clipCoordinates, mat4 projectionMatrix);
        vec3 toWorldCoordinates(vec4 viewCoordinates, mat4 viewMatrix);

    private:

        // Variables
        bool isPicking = false;
        int mouseX = 0;
        int mouseY = 0;
        Camera* camera;

    protected:

};

#endif