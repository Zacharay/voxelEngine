#pragma once
#include <memory>
#include <glm/vec3.hpp>
#include <Shader.hpp>
#include <vector>
#include <World.hpp>




class MeshRenderer {

    unsigned int m_textureAtlas;

    std::unique_ptr<Shader> shader;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    public:
    MeshRenderer();

    void renderChunks(const ChunkMap &chunks)const;
    void setViewMatrix(const glm::mat4 &viewMatrix);

};