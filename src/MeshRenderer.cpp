#include "MeshRenderer.hpp"
#include "Shader.hpp"
#include "Config.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

MeshRenderer::MeshRenderer() {
    shader = std::make_unique<Shader>("blockVertexShader.vert", "blockFragmentShader.frag");
    m_viewMatrix = glm::mat4(1.0f);
    m_viewMatrix = glm::translate(m_viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

    constexpr float aspectRatio = static_cast<float>(Config::windowWidth) / static_cast<float>(Config::windowHeight);
    m_projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);




}
void MeshRenderer::setViewMatrix(const glm::mat4 &viewMatrix) {
    m_viewMatrix = viewMatrix;
}

void MeshRenderer::renderMesh(const std::vector<Face> &faces)const {


    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, faces.size() * sizeof(Face), faces.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

    shader->useProgram();

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    shader->setMat4(modelMatrix,"model");
    shader->setMat4(m_projectionMatrix,"projection");
    shader->setMat4(m_viewMatrix,"view");

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, faces.size() * 6 );

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

