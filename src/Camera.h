#pragma once

#include "Maths.h"

struct Transform {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
};

class Camera {
  public:
    Camera();

    void mouseInput(int xOffset, int yOffset);
    void keyboardInput();

    glm::mat4 update();

    Transform transform;

  private:
    glm::mat4 m_projectionMatrix{1.0f};

    glm::vec3 m_up{0.0f};
    glm::vec3 m_front{0.0f};
};
