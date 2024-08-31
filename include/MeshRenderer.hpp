#pragma once
#include <memory>
#include <glm/vec3.hpp>
#include <Shader.hpp>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};
struct Face {
    Vertex vertices[6];
};



class MeshRenderer {
    std::unique_ptr<Shader> shader;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    public:
    MeshRenderer();

    void renderMesh(const std::vector<Face>&faces)const;
    void setViewMatrix(const glm::mat4 &viewMatrix);

};