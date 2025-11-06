#pragma once

#include <memory>
#include <Shader.hpp>
#include <World.hpp>




class MeshRenderer {

    unsigned int m_textureAtlas;

    std::unique_ptr<Shader> m_solidShader;
    std::unique_ptr<Shader> m_transparentShader;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    public:
    MeshRenderer();

    void renderSolidChunks(const ChunkMap &chunks)const;
    void renderTransparentChunks(const ChunkMap &chunks)const;
    void setViewMatrix(const glm::mat4 &viewMatrix);

};