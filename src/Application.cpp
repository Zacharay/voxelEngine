#include  "Application.hpp"

#include <iostream>
#include <Timer.hpp>
#include "Camera.hpp"


static void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    if (auto app = static_cast<Application*>(glfwGetWindowUserPointer(window))) {
        app->onMouseMovement(xPos, yPos);
    }
}


Application::Application() : Window(){
    m_world = std::make_unique<World>();
    m_meshRenderer = std::make_unique<MeshRenderer>();
    m_playerController = std::make_unique<PlayerController>(glm::vec3(0.0f, 0.0f, 0.0f));

    m_lastFrame = static_cast<float>(glfwGetTime());

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetCursorPosCallback(m_window, mouse_callback);
}

void Application::onMouseMovement(const float xPos,const float yPos) {
    static bool firstMouseMove = false;
    static float lastX = 450.0f,lastY=450.0f;
    const float mouseSensitivity = 0.05f;


    if(firstMouseMove) {
        lastX = xPos;
        lastY = yPos;
        firstMouseMove = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;


    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    m_playerController->handleMouseMovement(xOffset,yOffset);

}

void Application::onRender() {

    const ChunkMap &chunks = m_world->getChunks();

    m_meshRenderer->setViewMatrix(m_playerController->getViewMatrix());

    m_meshRenderer->renderSkybox();

    m_meshRenderer->renderSolidChunks(chunks);
    m_meshRenderer->renderTransparentChunks(chunks);


}
void Application::onUpdate() {
     const double currentFrame = glfwGetTime();
     const double deltaTime = currentFrame - m_lastFrame;
     m_lastFrame = currentFrame;

     m_playerController->update(static_cast<float>(deltaTime),m_window,*m_world);

     const glm::vec3 playerPos = m_playerController->getPosition();

     int playerChunkX = static_cast<int>(playerPos.x) / static_cast<int>(Config::chunkSize);
     int playerChunkZ = static_cast<int>(playerPos.z) / static_cast<int>(Config::chunkSize);

    try {
        for (int x = playerChunkX - Config::chunkRadius; x < playerChunkX + Config::chunkRadius; x++) {
            for (int z = playerChunkZ - Config::chunkRadius; z < playerChunkZ + Config::chunkRadius; z++) {
                m_world->loadChunk(x, z);
            }
        }

        m_world->unloadFarChunks(playerChunkX, playerChunkZ);
        m_world->processChunkMeshes();
    } catch (const std::exception &e) {
        std::cerr << "Exception during update: " << e.what() << std::endl;
        glfwSetWindowShouldClose(m_window, true);
    }
}
