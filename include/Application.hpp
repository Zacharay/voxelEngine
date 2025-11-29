#pragma once
#include "Camera.hpp"
#include "Window.hpp"
#include "World.hpp"
#include "MeshRenderer.hpp"

#include "PlayerController.hpp"

class Application : public Window{
public:
    void onMouseMovement(float xPos,float yPos);
    Application();
protected:
       void onUpdate()override;
       void onRender()override;
private:
    std::unique_ptr<World> m_world;
    std::unique_ptr<MeshRenderer> m_meshRenderer;
    std::unique_ptr<PlayerController> m_playerController;

    double m_lastFrame ;
    void processInput(double deltaTime);
};
