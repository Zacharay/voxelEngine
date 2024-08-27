#pragma once
#include <memory>
#include <glm/vec3.hpp>
#include <Shader.hpp>
#include <vector>
struct Face {
    glm::vec3 vertexPositions[6];
    glm::vec2 textureCoordinates[6];
};

class MeshRenderer {
    std::unique_ptr<Shader> shader;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    public:
    MeshRenderer();

    void renderMesh(std::vector<Face>&faces);
    void setViewMatrix(const glm::mat4 &viewMatrix);

};