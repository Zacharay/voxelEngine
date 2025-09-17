#include "MeshRenderer.hpp"
#include "Shader.hpp"
#include "Config.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <World.hpp>

#include "ChunkColumn.hpp"
#include "stb_image.h"

MeshRenderer::MeshRenderer() {
    shader = std::make_unique<Shader>("blockVertexShader.vert", "blockFragmentShader.frag");
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

void MeshRenderer::renderChunks(const ChunkMap &chunks)const {


    shader->useProgram();
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureAtlas);
    shader->setInt(0, "textureAtlas");
    shader->setMat4(modelMatrix,"model");
    shader->setMat4(m_projectionMatrix,"projection");
    shader->setMat4(m_viewMatrix,"view");
    int counter = 0;
    for(auto &chunk : chunks) {
        chunk.second.bindMesh();
        glDrawArrays(GL_TRIANGLES, 0, chunk.second.m_meshSize * 6 );
        counter +=  chunk.second.m_meshSize ;
    }
    if(Config::showFaceCount) {
        std::cout<<counter<<std::endl;
    }

}

