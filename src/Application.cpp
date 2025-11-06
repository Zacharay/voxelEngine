#include  "Application.hpp"

#include <iostream>
#include <Timer.hpp>
#include "Camera.hpp"


static float lastX = 450.0f,lastY=450.0f;
bool firstMouse = true;
std::unique_ptr<Camera> camera= std::unique_ptr<Camera>(new Camera(glm::vec3(0.0f, 200.0f, 500.0f), glm::vec3(0.0f, 0.0f, -1.0f),glm::vec3(0.0f,1.0f,0.0f)));


static void mouse_callback(GLFWwindow* window, double xPos,double yPos) {
    if(firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    const float sensitivity = 0.05f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    camera->processMouseInput(xOffset,yOffset);
}

Application::Application() : Window(){
    world = std::make_unique<World>();
    meshRenderer = std::make_unique<MeshRenderer>();

    m_lastFrame = static_cast<float>(glfwGetTime());

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(m_window, mouse_callback);
}
void Application::processInput(double deltaTime) {

    if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {

        camera->processKeyboardInput(Camera_Movement::FORWARD,deltaTime);
    }
    if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->processKeyboardInput(Camera_Movement::BACKWARDS,deltaTime);
    }
    if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->processKeyboardInput(Camera_Movement::LEFT,deltaTime);
    }
    if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->processKeyboardInput(Camera_Movement::RIGHT,deltaTime);
    }
    if(glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera->processKeyboardInput(Camera_Movement::UP,deltaTime);
    }
    if(glfwGetKey(m_window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera->processKeyboardInput(Camera_Movement::DOWN,deltaTime);
    }
    if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window,true);
    }
}


void Application::onRender() {

    const ChunkMap &chunks = world->getChunks();

    meshRenderer->setViewMatrix(camera->getViewMatrix());

    meshRenderer->renderSkybox();

    meshRenderer->renderSolidChunks(chunks);
    meshRenderer->renderTransparentChunks(chunks);


}
void Application::onUpdate() {
     const double currentFrame = glfwGetTime();
     const double deltaTime = currentFrame - m_lastFrame;
     m_lastFrame = currentFrame;

     processInput(deltaTime);

     glm::vec3 playerPos = camera->getPosition();

     int playerChunkX = static_cast<int>(playerPos.x) / static_cast<int>(Config::chunkSize);
     int playerChunkZ = static_cast<int>(playerPos.z) / static_cast<int>(Config::chunkSize);

    for(int x = playerChunkX - Config::chunkRadius; x < playerChunkX + Config::chunkRadius; x++) {
        for( int z = playerChunkZ - Config::chunkRadius; z < playerChunkZ + Config::chunkRadius; z++) {
            world->loadChunk(x,z);
        }
    }

    world->unloadFarChunks(playerChunkX,playerChunkZ);

    world->regenerateMeshes();
}
