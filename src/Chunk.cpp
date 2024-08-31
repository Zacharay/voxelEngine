#include "Chunk.hpp"

#include <iostream>

// Front face (Z+)
constexpr glm::vec3 frontFace[6] = {
    glm::vec3(0.5f, -0.5f, 0.5f),  // Bottom-right
    glm::vec3(-0.5f, -0.5f, 0.5f), // Bottom-left
    glm::vec3(-0.5f, 0.5f, 0.5f),  // Top-left
    glm::vec3(-0.5f, 0.5f, 0.5f),  // Top-left
    glm::vec3(0.5f, 0.5f, 0.5f),   // Top-right
    glm::vec3(0.5f, -0.5f, 0.5f)   // Bottom-right
};

// Back face (Z-)
constexpr glm::vec3 backFace[6] = {
    glm::vec3(0.5f, -0.5f, -0.5f),  // Bottom-right
    glm::vec3(-0.5f, -0.5f, -0.5f), // Bottom-left
    glm::vec3(-0.5f, 0.5f, -0.5f),  // Top-left
    glm::vec3(-0.5f, 0.5f, -0.5f),  // Top-left
    glm::vec3(0.5f, 0.5f, -0.5f),   // Top-right
    glm::vec3(0.5f, -0.5f, -0.5f)   // Bottom-right
};

// Left face (X-)
constexpr glm::vec3 leftFace[6] = {
    glm::vec3(-0.5f, -0.5f, 0.5f),  // Bottom-right
    glm::vec3(-0.5f, -0.5f, -0.5f), // Bottom-left
    glm::vec3(-0.5f, 0.5f, -0.5f),  // Top-left
    glm::vec3(-0.5f, 0.5f, -0.5f),  // Top-left
    glm::vec3(-0.5f, 0.5f, 0.5f),   // Top-right
    glm::vec3(-0.5f, -0.5f, 0.5f)   // Bottom-right
};

// Right face (X+)
constexpr glm::vec3 rightFace[6] = {
    glm::vec3(0.5f, -0.5f, 0.5f),  // Bottom-right
    glm::vec3(0.5f, -0.5f, -0.5f), // Bottom-left
    glm::vec3(0.5f, 0.5f, -0.5f),  // Top-left
    glm::vec3(0.5f, 0.5f, -0.5f),  // Top-left
    glm::vec3(0.5f, 0.5f, 0.5f),   // Top-right
    glm::vec3(0.5f, -0.5f, 0.5f)   // Bottom-right
};

// Top face (Y+)
constexpr glm::vec3 topFace[6] = {
    glm::vec3(-0.5f, 0.5f, 0.5f),  // Bottom-left
    glm::vec3(0.5f, 0.5f, 0.5f),   // Bottom-right
    glm::vec3(0.5f, 0.5f, -0.5f),  // Top-right
    glm::vec3(0.5f, 0.5f, -0.5f),  // Top-right
    glm::vec3(-0.5f, 0.5f, -0.5f), // Top-left
    glm::vec3(-0.5f, 0.5f, 0.5f)   // Bottom-left
};

// Bottom face (Y-)
constexpr glm::vec3 bottomFace[6] = {
    glm::vec3(-0.5f, -0.5f, 0.5f),  // Bottom-left
    glm::vec3(0.5f, -0.5f, 0.5f),   // Bottom-right
    glm::vec3(0.5f, -0.5f, -0.5f),  // Top-right
    glm::vec3(0.5f, -0.5f, -0.5f),  // Top-right
    glm::vec3(-0.5f, -0.5f, -0.5f), // Top-left
    glm::vec3(-0.5f, -0.5f, 0.5f)   // Bottom-left
};


Chunk::Chunk(const int x,const int y,const int z):blocks{},
    m_chunkPositionX(x),
    m_chunkPositionY(y),
    m_chunkPositionZ(z)
{

    for(int i=0;i<Config::chunkSize;i++)
        for(int j=0;j<Config::chunkSize;j++)
            for(int k=0;k<Config::chunkSize;k++) {
                blocks[i][j][k] = 0;
            }

}
void Chunk::generateMesh() {
    for(int z=0;z<Config::chunkSize;z++)
        for(int y=0;y<Config::chunkSize;y++) {
            for(int x=0;x<Config::chunkSize;x++) {
                glm::vec3 offsetVec = glm::vec3(
                static_cast<float>(m_chunkPositionX)*16.0f+x,
                static_cast<float>(m_chunkPositionY)*16.0f+y,
                static_cast<float>(m_chunkPositionZ)*16.0f+z);

            Face faceFront;
            Face faceBack;
            Face faceLeft;
            Face faceRight;
            Face faceTop;
            Face faceBottom;


            glm::vec3 color ;
            if(m_chunkPositionY==0) {
                color = glm::vec3(34.0f/255.0f,139.0f/255.0f,34.0f/255.0f);
            }
            else {
                color = glm::vec3(34.0f/255.0f,139.0f/255.0f,34.0f/255.0f);
                //color = glm::vec3(237.0f/255.0f,201.0f/255.0f,175.0f/255.0f);
            }

            for (int i = 0; i < 6; i++)
            {
                faceFront.vertices[i].position = frontFace[i] + offsetVec;
                faceFront.vertices[i].color = color;

                faceLeft.vertices[i].position = leftFace[i] + offsetVec;
                faceLeft.vertices[i].color = color;

                faceBack.vertices[i].position = backFace[i] + offsetVec;
                faceBack.vertices[i].color = color;

                faceRight.vertices[i].position = rightFace[i] + offsetVec;
                faceRight.vertices[i].color = color;

                faceTop.vertices[i].position = topFace[i] + offsetVec;
                faceTop.vertices[i].color = color;

                faceBottom.vertices[i].position = bottomFace[i] + offsetVec;
                faceBottom.vertices[i].color = color;
            }

            mesh.push_back(faceFront);
            mesh.push_back(faceLeft);
            mesh.push_back(faceBack);
            mesh.push_back(faceRight);
            mesh.push_back(faceTop);
            mesh.push_back(faceBottom);

        }
    }

}
const std::vector<Face> &Chunk::getMesh()const {


    return mesh;
}