#include "MeshRenderer.hpp"
#include "Shader.hpp"
#include "Config.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <World.hpp>

#include "ChunkColumn.hpp"
#include "stb_image.h"

MeshRenderer::MeshRenderer() {
    m_solidShader = std::make_unique<Shader>("blockVertexShader.vert", "blockFragmentShader.frag");
    m_transparentShader = std::make_unique<Shader>("transparentVertexShader.vert", "transparentFragmentShader.frag");
    m_viewMatrix = glm::mat4(1.0f);
    m_viewMatrix = glm::translate(m_viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

    constexpr float aspectRatio = static_cast<float>(Config::windowWidth) / static_cast<float>(Config::windowHeight);
    m_projectionMatrix = glm::perspective(glm::radians(90.0f), aspectRatio, 0.05f, 20000.0f);

    int width,height,channels;


    unsigned char *data = stbi_load("textures/textureAtlas.png",&width,&height,&channels,STBI_rgb_alpha);


    glGenTextures(1,&m_textureAtlas);
    glBindTexture(GL_TEXTURE_2D,m_textureAtlas);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


}
void MeshRenderer::setViewMatrix(const glm::mat4 &viewMatrix) {
    m_viewMatrix = viewMatrix;
}

void MeshRenderer::renderSolidChunks(const ChunkMap &chunks)const {

    m_solidShader->useProgram();
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureAtlas);
    m_solidShader->setInt(0, "textureAtlas");
    m_solidShader->setMat4(modelMatrix,"model");
    m_solidShader->setMat4(m_projectionMatrix,"projection");
    m_solidShader->setMat4(m_viewMatrix,"view");
    int counter = 0;
    for(auto &chunk : chunks) {

            chunk.second.bindSolidMesh();
            glDrawArrays(GL_TRIANGLES, 0, chunk.second.getSolidMeshSize() * 6 );
            counter +=  chunk.second.getSolidMeshSize() ;


    }
    if(Config::showFaceCount) {
        std::cout<<counter<<std::endl;
    }

}
void MeshRenderer::renderTransparentChunks(const ChunkMap &chunks)const {
    m_transparentShader->useProgram();
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureAtlas);

    m_transparentShader->setFloat((float)glfwGetTime(),"time");
    m_transparentShader->setInt(0, "textureAtlas");
    m_transparentShader->setMat4(modelMatrix,"model");
    m_transparentShader->setMat4(m_projectionMatrix,"projection");
    m_transparentShader->setMat4(m_viewMatrix,"view");


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    for(auto &chunk : chunks) {
        if(!chunk.second.isMeshDirty()) {
            chunk.second.bindTransparentMesh();
            glDrawArrays(GL_TRIANGLES, 0, chunk.second.getTransparentMeshSize() * 6 );

        }

    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}
