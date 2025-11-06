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
    m_skyboxShader = std::make_unique<Shader>("skyboxVertexShader.vert", "skyboxFragmentShader.frag");
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

    std::vector<std::string>faces ={
        "textures/cubemap_0.png",//
        "textures/cubemap_2.png",
        "textures/cubemap_4.png",
        "textures/cubemap_4.png",
        "textures/cubemap_3.png",
        "textures/cubemap_1.png"//
    };
    std::vector<std::string> faces1{
        "textures/right.jpg",
        "textures/left.jpg",
        "textures/top.jpg",
        "textures/bottom.jpg",
        "textures/front.jpg",
        "textures/back.jpg"
    };
    m_skyboxTexture = loadCubemap(faces);
    prepareSkyboxVao();
}
void MeshRenderer::setViewMatrix(const glm::mat4 &viewMatrix) {
    m_viewMatrix = viewMatrix;
}
void MeshRenderer::renderSkybox() const {
    glDepthMask(GL_FALSE);

    m_skyboxShader->useProgram();

    glm::mat4 viewBezTranslacji = glm::mat4(glm::mat3(m_viewMatrix));

    m_skyboxShader->setMat4(m_projectionMatrix,"projection");
    m_skyboxShader->setMat4(viewBezTranslacji,"view");

    glBindVertexArray(m_skyboxVao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
}
void MeshRenderer::prepareSkyboxVao() {
    float skyboxVertices[] = {
        // positions
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
    glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(float), skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0);
}

unsigned int MeshRenderer::loadCubemap(std::vector<std::string> faces) {

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        // === POCZĄTEK POPRAWKI ===

        // Zmuszamy stb_image do załadowania obrazu ZAWSZE jako 4-kanałowy (RGBA / 32-bit).
        // STBI_rgb_alpha to stała (zdefiniowana jako 4) z stb_image.h
        // Jeśli plik to JPG (3 kanały), stb_image automatycznie doda mu kanał alfa (o wartości 255).

        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

        // === KONIEC POPRAWKI ===

        if (data)
        {
            // Skoro teraz mamy GWARANCJĘ, że dane są zawsze w formacie RGBA,
            // możemy na sztywno ustawić formaty dla OpenGL na GL_RGBA.

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            // stbi_image_free(data); // Niepotrzebne, data jest nullptr
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
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
