#pragma once

#include "Maths.h"

struct Camera {
    glm::mat4 projectionMatrix{1.0f};

    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};

    glm::vec3 up{0.0f};
    glm::vec3 front{0.0f};
};

struct Camera createCamera();
void cameraKeyboardInput(struct Camera* camera);
void cameraMouseInput(struct Camera* camera, uint32_t xrel, uint32_t yrel);
void cameraUpdate(struct Camera* camera, glm::mat4& projectionViewMatrix);
