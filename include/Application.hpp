#include "Camera.hpp"
#include "Window.hpp"
#include "World.hpp"
#include "MeshRenderer.hpp"
class Application : public Window{

    std::unique_ptr<World> world;
    std::unique_ptr<MeshRenderer> meshRenderer;


    float m_lastFrame ;
    void processInput(float deltaTime);

public:
    Application();
protected:

       void onUpdate()override;
       void onRender()override;
};