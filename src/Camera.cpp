#include "Camera.hpp"

#include <glm/ext/matrix_transform.hpp>


Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) {
    m_position = position;
    m_front = front;
    m_up = up;

    yaw = -90.0f;
    pitch = 0.0f;

}
void Camera::processKeyboardInput(Camera_Movement direction, float deltaTime) {
    const float cameraSpeed = 20.0f * deltaTime;

    glm::vec3 right = glm::normalize(glm::cross(m_front, m_up));

    if(Camera_Movement::FORWARD == direction) {
        m_position += m_front * cameraSpeed;
    }
    else if(Camera_Movement::BACKWARDS == direction) {
        m_position -= m_front * cameraSpeed;
    }
    else if (Camera_Movement::RIGHT == direction) {
        m_position += right * cameraSpeed;
    }
    else if(Camera_Movement::LEFT == direction) {
        m_position -= right * cameraSpeed;
    }
    else if(Camera_Movement::UP == direction) {
        m_position += m_up * cameraSpeed;
    }
    else if(Camera_Movement::DOWN == direction) {
        m_position -= m_up * cameraSpeed;
    }
}

void Camera::processMouseInput(float xOffset, float yOffset) {
    yaw   += xOffset;
    pitch += yOffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_front = glm::normalize(direction);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}
