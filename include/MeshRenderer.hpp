#pragma once

#include <memory>
#include <Shader.hpp>
#include <World.hpp>




class MeshRenderer {
public:
    MeshRenderer();

    void setViewMatrix(const glm::mat4 &viewMatrix);
    void renderSolidChunks(const ChunkMap &chunks)const;
    void renderTransparentChunks(const ChunkMap &chunks)const;
    void renderSkybox()const;
private:
    void prepareSkyboxVao();
    static unsigned int loadSkyboxTexture(const std::array<std::string,6>& faces);
    static unsigned int loadTextureAtlas(const std::string& path);

    unsigned int m_skyboxVao{}, m_skyboxVbo{};

    unsigned int m_textureAtlas;
    unsigned int m_skyboxTexture;

    std::unique_ptr<Shader> m_solidShader;
    std::unique_ptr<Shader> m_transparentShader;
    std::unique_ptr<Shader> m_skyboxShader;

    glm::mat4 m_viewMatrix{};
    glm::mat4 m_projectionMatrix{};

};