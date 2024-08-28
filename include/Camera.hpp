#pragma once
#include <glm/glm.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARDS,
    LEFT,
    RIGHT,
    DOWN,
    UP
};

class Camera {
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;

    float yaw;
    float pitch;



public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up);

    void processKeyboardInput(Camera_Movement movement,float deltaTime);
    void processMouseInput(float xOffset, float yOffset);
    glm::mat4 getViewMatrix()const;

};