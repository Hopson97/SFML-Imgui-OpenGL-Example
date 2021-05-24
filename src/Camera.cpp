#include "Camera.h"

#include "Utility.h"
#include <stdio.h>

struct Camera createCamera()
{
    float aspect = (float)WIDTH / (float)HEIGHT;

    Camera camera;
    camera.projectionMatrix = createProjectionMatrix(aspect, 90.0f);

    camera.rotation.y = -90.0f;
    camera.up.y = 1.0f;

    return camera;
}

void cameraKeyboardInput(struct Camera* camera)
{
    // const uint8_t* keyboard = SDL_GetKeyboardState(NULL);
    //// Keyboard input
    //// https://wiki.libsdl.org/SDL_Scancode
    // if (keyboard[SDL_SCANCODE_W]) {
    //    moveVectorForwards(camera->position, camera->rotation, 1);
    //}
    // if (keyboard[SDL_SCANCODE_A]) {
    //    moveVectorLeft(camera->position, camera->rotation, 1);
    //}
    // if (keyboard[SDL_SCANCODE_S]) {
    //    moveVectorBackwards(camera->position, camera->rotation, 1);
    //}
    // if (keyboard[SDL_SCANCODE_D]) {
    //    moveVectorRight(camera->position, camera->rotation, 1);
    //}
}

void cameraMouseInput(struct Camera* camera, uint32_t xrel, uint32_t yrel)
{
    int mouseXDiff = xrel;
    int mouseYDiff = yrel;
    camera->rotation.x -= mouseYDiff / 4.0f;
    camera->rotation.y += mouseXDiff / 4.0f;
    camera->rotation.x = glm::clamp(camera->rotation.x, -89.9f, 89.9f);
}

void cameraUpdate(struct Camera* camera, glm::mat4& projectionViewMatrix)
{
    // View Matrix
    glm::vec3 center;

    camera->front.x = cos(glm::radians(camera->rotation.y)) * cos(glm::radians(camera->rotation.x));
    camera->front.y = sin(glm::radians(camera->rotation.x));
    camera->front.z = sin(glm::radians(camera->rotation.y)) * cos(glm::radians(camera->rotation.x));
    camera->front = glm::normalize(camera->front);

    center = camera->position + camera->front;

    glm::mat4 viewMatrix = glm::lookAt(camera->position, center, camera->up);

    // Calculate projection view matrix and then upload
    projectionViewMatrix = camera->projectionMatrix * viewMatrix;
}
