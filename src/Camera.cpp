#include "Camera.h"

#include "Utility.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

Camera::Camera()
{
    float aspect = (float)WIDTH / (float)HEIGHT;
    m_projectionMatrix = createProjectionMatrix(aspect, 90.0f);
    transform.rotation.y = -90.0f;
    m_up.y = 1.0f;
}

void Camera::mouseInput(int xOffset, int yOffset)
{
    static sf::Vector2i m_lastMousePosition;
    sf::Vector2i change = sf::Vector2i(xOffset, yOffset) - m_lastMousePosition;
    transform.rotation.x -= static_cast<float>(change.y / 4.0f);
    transform.rotation.y += static_cast<float>(change.x / 4.0f);
    m_lastMousePosition = {xOffset, yOffset};

    transform.rotation.x = glm::clamp(transform.rotation.x, -89.9f, 89.9f);
}

void Camera::keyboardInput()
{
    const float FACTOR = 0.25f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        transform.position += forwardsVector(transform.rotation) * FACTOR;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        transform.position += backwardsVector(transform.rotation) * FACTOR;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        transform.position += leftVector(transform.rotation) * FACTOR;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        transform.position += rightVector(transform.rotation) * FACTOR;
    }
}

glm::mat4 Camera::update()
{
    // View Matrix
    glm::vec3 center;

    m_front.x = cos(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
    m_front.y = sin(glm::radians(transform.rotation.x));
    m_front.z = sin(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
    m_front = glm::normalize(m_front);

    center = transform.position + m_front;

    glm::mat4 viewMatrix = glm::lookAt(transform.position, center, m_up);

    // Calculate projection view matrix and then upload
    return m_projectionMatrix * viewMatrix;
}
