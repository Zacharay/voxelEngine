#pragma once

#include <memory>
#include <Shader.hpp>
#include <World.hpp>




class MeshRenderer {

    unsigned int m_skyboxVao, m_skyboxVbo;

    unsigned int m_textureAtlas;
    unsigned int m_skyboxTexture;

    std::unique_ptr<Shader> m_solidShader;
    std::unique_ptr<Shader> m_transparentShader;
    std::unique_ptr<Shader> m_skyboxShader;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    public:
    MeshRenderer();


    void prepareSkyboxVao();
    void renderSkybox()const;
    unsigned int loadCubemap(std::vector<std::string> faces);

    void renderSolidChunks(const ChunkMap &chunks)const;
    void renderTransparentChunks(const ChunkMap &chunks)const;
    void setViewMatrix(const glm::mat4 &viewMatrix);

};