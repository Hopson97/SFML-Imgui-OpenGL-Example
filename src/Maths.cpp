#include "Maths.h"

glm::mat4 createModelMatrix(const glm::vec3& pos, const glm::vec3& rot)
{
    glm::mat4 matrix{1.0f};

    matrix = glm::translate(matrix, pos);

    matrix = glm::rotate(matrix, glm::radians(rot.x), {1, 0, 0});
    matrix = glm::rotate(matrix, glm::radians(rot.y), {0, 1, 0});
    matrix = glm::rotate(matrix, glm::radians(rot.z), {0, 0, 1});
    return matrix;
}

glm::mat4 createProjectionMatrix(float aspectRatio, float fov)
{
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 10000.0f);
}

glm::vec3 forwardsVector(const glm::vec3& rotation)
{
    float yaw = glm::radians(rotation.y);
    float pitch = glm::radians(rotation.x);

    return {glm::cos(yaw) * glm::cos(pitch), glm::sin(pitch), glm::cos(pitch) * glm::sin(yaw)};
}

glm::vec3 backwardsVector(const glm::vec3& rotation)
{
    return -forwardsVector(rotation);
}

glm::vec3 leftVector(const glm::vec3& rotation)
{
    float yaw = glm::radians(rotation.y + 90);
    return {-glm::cos(yaw), 0, -glm::sin(yaw)};
}

glm::vec3 rightVector(const glm::vec3& rotation)
{
    return -leftVector(rotation);
}