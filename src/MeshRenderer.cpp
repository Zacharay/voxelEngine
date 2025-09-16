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


}
void MeshRenderer::setViewMatrix(const glm::mat4 &viewMatrix) {
    m_viewMatrix = viewMatrix;
}

void MeshRenderer::renderChunks(const ChunkMap &chunks)const {

    shader->useProgram();
    glm::mat4 modelMatrix = glm::mat4(1.0f);

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

