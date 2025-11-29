#pragma once
#include <Camera.hpp>
#include <memory>
#include <World.hpp>
#include <GLFW/glfw3.h>

class PlayerController {
public:
    PlayerController(glm::vec3 startPos);

    void handleMouseMovement(float xOffset,float yOffset);
    void update(float deltaTime,GLFWwindow* window,World &world);

    [[nodiscard]] const glm::vec3& getPosition() const;
    [[nodiscard]] const glm::mat4& getViewMatrix() const;

private:
    std::unique_ptr<Camera>m_camera;



    const float MOUSE_INTERACTION_REACH = 5.0f;
    const float BREAK_COOLDOWN = 0.1f;
    const float PLACE_COOLDOWN = 0.2f;
    const float SELECT_COOLDOWN = 0.1f;

    float m_mouseInteractionTimer = 0.0f;
    BlockType m_selectedBlock;

    void handleMovement(GLFWwindow *window,float deltaTime);
    void handleInventory(GLFWwindow*window ,World &world);
    void handleMouseInteraction(GLFWwindow*window ,World &world);
    RaycastResult getTargetBlock(World &world)const;

};
