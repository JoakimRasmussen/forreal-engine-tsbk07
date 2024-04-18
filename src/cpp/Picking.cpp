
#include "../h/Picking.h"


Picking::Picking(Camera* camera) {
    // Constructor
    this->camera = camera;
}

Picking::~Picking() {
    // Destructor
}

void Picking::update() {
    // Methods
}

vec3 Picking::calculateMouseRay(float mouseX, float mouseY, int screenWidth, int screenHeight) {
    // Ray casting
    vec3 rayNormalized = normalizeScreenCoordinates(mouseX, mouseY, screenWidth, screenHeight);
    vec4 rayClip = vec4(rayNormalized.x, rayNormalized.y, -1.0f, 1.0f);
    vec4 rayView = toViewCoordinates(rayClip, camera->getProjectionMatrix());
    vec3 ray = toWorldCoordinates(rayView, camera->getViewMatrix());
    // Normalize the vector
    ray = normalize(ray);
    return ray;
}

vec3 Picking::normalizeScreenCoordinates(float mouseX, float mouseY, int screenWidth, int screenHeight) {
    // normalize
    float x = (2.0f * mouseX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / screenHeight;
    float z = 1.0f;
    return vec3(x, y, z);
}

vec4 Picking::toViewCoordinates(vec4 rayClip, mat4 projectionMatrix) {
    vec4 viewCoordinates = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    // view
    vec4 eyeCoordinates = inverse(projectionMatrix) * rayClip;
    viewCoordinates = vec4(eyeCoordinates.x, eyeCoordinates.y, -1.0f, 0.0f);
    return viewCoordinates;
}

vec3 Picking::toWorldCoordinates(vec4 viewCoordinates, mat4 viewMatrix) {
    vec3 worldCoordinates = vec3(0.0f, 0.0f, 0.0f);
    // world
    vec4 rayWorld = inverse(viewMatrix) * viewCoordinates;
    worldCoordinates = vec3(rayWorld.x, rayWorld.y, rayWorld.z);
    return worldCoordinates;
}

vec3 Picking::getIntersectionPoint() {
    return intersectionPoint;
}

void Picking::updateIsPicking(bool isPicking) {
    this->isPicking = isPicking;
}

void Picking::setLastColor(float color[4]) {
    lastColor[0] = color[0];
    lastColor[1] = color[1];
    lastColor[2] = color[2];
    lastColor[3] = color[3];
}

bool Picking::isHit(const std::array<float, 3>& inputColor) {
    // Check each color component (R, G, B) against the stored `lastColor` within `epsilon`
    return (std::abs(inputColor[0] - lastColor[0]) < epsilon &&
            std::abs(inputColor[1] - lastColor[1]) < epsilon &&
            std::abs(inputColor[2] - lastColor[2]) < epsilon);
}
