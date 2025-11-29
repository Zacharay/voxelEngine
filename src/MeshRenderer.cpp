#include "MeshRenderer.hpp"
#include "Shader.hpp"
#include "Config.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <World.hpp>

#include "ChunkColumn.hpp"
#include "stb_image.h"

const std::array<std::string,6>skyboxFilepathFaces ={
    "textures/cubemap_0.png",
    "textures/cubemap_2.png",
    "textures/cubemap_4.png",
    "textures/cubemap_4.png",
    "textures/cubemap_3.png",
    "textures/cubemap_1.png"
};

MeshRenderer::MeshRenderer() {
    m_solidShader = std::make_unique<Shader>("shaders/blockVertexShader.vert", "shaders/blockFragmentShader.frag");
    m_transparentShader = std::make_unique<Shader>("shaders/transparentVertexShader.vert", "shaders/transparentFragmentShader.frag");
    m_skyboxShader = std::make_unique<Shader>("shaders/skyboxVertexShader.vert", "shaders/skyboxFragmentShader.frag");

    m_viewMatrix = glm::mat4(1.0f);
    m_viewMatrix = glm::translate(m_viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
    constexpr float aspectRatio = static_cast<float>(Config::windowWidth) / static_cast<float>(Config::windowHeight);
    m_projectionMatrix = glm::perspective(glm::radians(90.0f), aspectRatio, 0.05f, 10000000.0f);


    m_textureAtlas = loadTextureAtlas("textures/textureAtlas.png");
    m_skyboxTexture = loadSkyboxTexture(skyboxFilepathFaces);
    prepareSkyboxVao();
}
void MeshRenderer::setViewMatrix(const glm::mat4 &viewMatrix) {
    m_viewMatrix = viewMatrix;
}
void MeshRenderer::prepareSkyboxVao() {
    constexpr unsigned int cubeVertexCount = 108;

    constexpr std::array<float,cubeVertexCount> skyboxVertices = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_skyboxVao);
    glGenBuffers(1, &m_skyboxVbo);
    glBindVertexArray(m_skyboxVao);


    glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVbo);
    glBufferData(GL_ARRAY_BUFFER, cubeVertexCount*sizeof(float), skyboxVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
}
unsigned int MeshRenderer::loadSkyboxTexture(const std::array<std::string,6>& faces){

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (uint32_t i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
unsigned int MeshRenderer::loadTextureAtlas(const std::string &path) {

    unsigned int textureID;
    int width,height,channels;

    unsigned char *data = stbi_load(path.c_str(),&width,&height,&channels,STBI_rgb_alpha);
    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_2D,textureID);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureID;
}
void MeshRenderer::renderSolidChunks(const ChunkMap &chunks)const {

    m_solidShader->useProgram();


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureAtlas);
    m_solidShader->setInt(0, "textureAtlas");

    m_solidShader->setMat4(m_projectionMatrix,"projection");
    m_solidShader->setMat4(m_viewMatrix,"view");
    unsigned int faceCountCounter = 0;
    for(auto &chunk : chunks) {
        ChunkColumn* column = chunk.second.get();

        auto modelMatrix = glm::mat4(1.0f);

        modelMatrix = glm::translate(modelMatrix,
            glm::vec3(
                static_cast<float>(column->m_posX) * Config::chunkSize,
                0.0f,
                static_cast<float>(column->m_posZ) * Config::chunkSize
        ));
        m_solidShader->setMat4(modelMatrix, "model");

        chunk.second->bindSolidMesh();
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(chunk.second->getSolidMeshSize()) * 6 );
        faceCountCounter +=  chunk.second->getSolidMeshSize() ;


    }
    if(Config::showFaceCount) {
        std::cout<<faceCountCounter<<std::endl;
    }

}
void MeshRenderer::renderTransparentChunks(const ChunkMap &chunks)const {
    m_transparentShader->useProgram();


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureAtlas);

    m_transparentShader->setFloat((float)glfwGetTime(),"time");
    m_transparentShader->setInt(0, "textureAtlas");
    m_transparentShader->setMat4(m_projectionMatrix,"projection");
    m_transparentShader->setMat4(m_viewMatrix,"view");


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    for(auto &chunk : chunks) {
        if(!chunk.second->isMeshDirty()) {
            ChunkColumn* column = chunk.second.get();

            auto modelMatrix = glm::mat4(1.0f);

             modelMatrix = glm::translate(modelMatrix,
                glm::vec3(
                    static_cast<float>(column->m_posX) * Config::chunkSize,
                    0.0f,
                    static_cast<float>(column->m_posZ) * Config::chunkSize
                ));
             m_transparentShader->setMat4(modelMatrix, "model");

            chunk.second->bindTransparentMesh();
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(chunk.second->getTransparentMeshSize()) * 6 );
        }

    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}
void MeshRenderer::renderSkybox() const {
    glDepthMask(GL_FALSE);

    m_skyboxShader->useProgram();

    glm::mat4 viewWithoutTranslation = glm::mat4(glm::mat3(m_viewMatrix));

    m_skyboxShader->setMat4(m_projectionMatrix,"projection");
    m_skyboxShader->setMat4(viewWithoutTranslation,"view");

    glBindVertexArray(m_skyboxVao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
}