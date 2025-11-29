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


    mutable glm::mat4 m_viewMatrix;
    mutable bool m_viewDirtyFlag;


    float yaw;
    float pitch;



public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up);

    void processKeyboardInput(Camera_Movement movement,double deltaTime);
    void processMouseInput(float xOffset, float yOffset);

    [[nodiscard]] const glm::mat4& getViewMatrix()const;
    [[nodiscard]] const glm::vec3& getPosition()const;
    [[nodiscard]] const glm::vec3& getFront()const;
};