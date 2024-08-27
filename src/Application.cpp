#include  "Application.hpp"

Application::Application() : Window(){
    world = std::unique_ptr<World>(new World());
    meshRenderer = std::unique_ptr<MeshRenderer>(new MeshRenderer());
}


void Application::onRender() {
    std::vector<Face> faces;
    meshRenderer->renderMesh(faces);
}
void Application::onUpdate() {

}
