#include "MeshRenderer.hpp"
#include "Shader.hpp"

MeshRenderer::MeshRenderer() {
    shader = std::unique_ptr<Shader>(new Shader("blockVertexShader.vert","blockFragmentShader.frag"));
    m_viewMatrix = glm::mat4(1.0f);
    m_viewMatrix = glm::translate(m_viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

    m_projectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
}
void MeshRenderer::setViewMatrix(const glm::mat4 &viewMatrix) {
    m_viewMatrix = viewMatrix;
}

void MeshRenderer::renderMesh(std::vector<Face> &faces) {
    Face face;

    face.vertexPositions[0] = glm::vec3(0.5f, -0.5f, 0.0f);
    face.vertexPositions[1] = glm::vec3(-0.5f, -0.5f, 0.0f);
    face.vertexPositions[2] = glm::vec3(-0.5f, 0.5f, 0.0f);
    face.vertexPositions[3] = glm::vec3(-0.5f, 0.5f, 0.0f);
    face.vertexPositions[4] = glm::vec3(0.5f, 0.5f, 0.0f);
    face.vertexPositions[5] = glm::vec3(0.5f, -0.5f, 0.0f);

    face.textureCoordinates[0] = glm::vec2(0.0f, 0.0f);
    face.textureCoordinates[1] = glm::vec2(0.0f, 0.0f);
    face.textureCoordinates[2] = glm::vec2(0.0f, 0.0f);
    face.textureCoordinates[3] = glm::vec2(0.0f, 0.0f);
    face.textureCoordinates[4] = glm::vec2(0.0f, 0.0f);
    face.textureCoordinates[5] = glm::vec2(0.0f, 0.0f);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    std::vector<float> vertexData;

        for (int i = 0; i < 6; ++i) {
            // Interleave vertex positions and texture coordinates
            vertexData.push_back(face.vertexPositions[i].x);
            vertexData.push_back(face.vertexPositions[i].y);
            vertexData.push_back(face.vertexPositions[i].z);
            vertexData.push_back(face.textureCoordinates[i].x);
            vertexData.push_back(face.textureCoordinates[i].y);
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

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 5);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

