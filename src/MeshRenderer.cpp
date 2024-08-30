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
    m_projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);



    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/grass_block_side.png", &width, &height, &nrChannels, 0);

    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA , GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);


    stbi_image_free(data);

    shader->useProgram();
    shader->setInt( 0,"mainTexture");

}
void MeshRenderer::setViewMatrix(const glm::mat4 &viewMatrix) {
    m_viewMatrix = viewMatrix;
}

void MeshRenderer::renderMesh(const std::vector<Face> &faces)const {


    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    std::vector<float> vertexData;
    for(const Face & face : faces) {
        for (int i = 0; i < 6; ++i) {
            // Interleave vertex positions and texture coordinates
            vertexData.push_back(face.vertexPositions[i].x);
            vertexData.push_back(face.vertexPositions[i].y);
            vertexData.push_back(face.vertexPositions[i].z);
            vertexData.push_back(face.textureCoordinates[i].x);
            vertexData.push_back(face.textureCoordinates[i].y);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    shader->useProgram();

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    shader->setMat4(modelMatrix,"model");
    shader->setMat4(m_projectionMatrix,"projection");
    shader->setMat4(m_viewMatrix,"view");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 5);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

