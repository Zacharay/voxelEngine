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

    unsigned int m_texture;
    public:
    MeshRenderer();

    void renderMesh(const std::vector<Face>&faces)const;
    void setViewMatrix(const glm::mat4 &viewMatrix);

};