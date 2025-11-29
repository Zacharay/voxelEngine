#include "PlayerController.hpp"

#include <ios>


PlayerController::PlayerController(glm::vec3 startPos):m_selectedBlock(BlockType::Air),m_mouseInteractionTimer(0) {
    m_camera = std::make_unique<Camera>(startPos,glm::vec3(0.0f, 0.0f, -1.0f),glm::vec3(0.0f,1.0f,0.0f));

}
void PlayerController::handleMouseMovement(const float xOffset,const float yOffset) {
    m_camera->processMouseInput(xOffset, yOffset);
}
void PlayerController::update(float deltaTime,GLFWwindow* window,World &world) {

    m_mouseInteractionTimer -= deltaTime;

    handleMovement(window,deltaTime);
    handleMouseInteraction(window,world);
    handleInventory(window,world);

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window,true);
    }
}
void PlayerController::handleMovement(GLFWwindow *window, float deltaTime) {

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

        m_camera->processKeyboardInput(Camera_Movement::FORWARD,deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        m_camera->processKeyboardInput(Camera_Movement::BACKWARDS,deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        m_camera->processKeyboardInput(Camera_Movement::LEFT,deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        m_camera->processKeyboardInput(Camera_Movement::RIGHT,deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        m_camera->processKeyboardInput(Camera_Movement::UP,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        m_camera->processKeyboardInput(Camera_Movement::DOWN,deltaTime);
    }

}
void PlayerController::handleMouseInteraction(GLFWwindow *window, World &world) {
    if(m_mouseInteractionTimer >0.0f)return;

    bool leftMouseButtonClicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    bool rightMouseButtonClicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    bool middleMouseButtonClicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;

    //prevent from raycasting if none of the mouseButtons is clicked
    if(!leftMouseButtonClicked && !rightMouseButtonClicked && !middleMouseButtonClicked)return;

    RaycastResult ray = getTargetBlock(world);

    if(ray.hit) {
        //break block
        if(leftMouseButtonClicked) {
            world.setBlockAt(ray.blockPos, BlockType::Air);

            m_mouseInteractionTimer = BREAK_COOLDOWN;
        }
        //place block
        else if(rightMouseButtonClicked)  {
            if(m_selectedBlock==BlockType::Air)return;

            const glm::ivec3 placePos = ray.blockPos + ray.faceNormal;
            world.setBlockAt(placePos, m_selectedBlock);
            m_mouseInteractionTimer = PLACE_COOLDOWN;
        }
        else {

            m_selectedBlock = ray.blockType;
            m_mouseInteractionTimer = SELECT_COOLDOWN;
        }
    }


}
void PlayerController::handleInventory(GLFWwindow *window, World &world) {
    if(glfwGetKey(window, GLFW_KEY_0)== GLFW_PRESS) {
        m_selectedBlock = BlockType::OAK_LOG;
    }
    if(glfwGetKey(window, GLFW_KEY_1)== GLFW_PRESS) {
        m_selectedBlock = BlockType::OAK_PLANKS;
    }
}
RaycastResult PlayerController::getTargetBlock(World &world) const {
    return world.rayCast(m_camera->getPosition(), m_camera->getFront(), MOUSE_INTERACTION_REACH);
}

const glm::vec3& PlayerController::getPosition()const {
    return m_camera->getPosition();
}
const glm::mat4& PlayerController::getViewMatrix()const {
    return m_camera->getViewMatrix();
}