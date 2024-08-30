#include "Chunk.hpp"

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

// Texture coordinates (shared across all faces)
constexpr glm::vec2 textureCoords[6] = {
    glm::vec2(1.0f, 0.0f), // Bottom-right
    glm::vec2(0.0f, 0.0f), // Bottom-left
    glm::vec2(0.0f, 1.0f), // Top-left
    glm::vec2(0.0f, 1.0f), // Top-left
    glm::vec2(1.0f, 1.0f), // Top-right
    glm::vec2(1.0f, 0.0f)  // Bottom-right
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





            for (int i = 0; i < 6; i++)
            {
                faceFront.vertexPositions[i] = frontFace[i] + offsetVec;
                faceFront.textureCoordinates[i] = textureCoords[i];

                faceLeft.vertexPositions[i] = leftFace[i] + offsetVec;
                faceLeft.textureCoordinates[i] = textureCoords[i];

                faceBack.vertexPositions[i] = backFace[i] + offsetVec;
                faceBack.textureCoordinates[i] = textureCoords[i];

                faceRight.vertexPositions[i] = rightFace[i] + offsetVec;
                faceRight.textureCoordinates[i] = textureCoords[i];

                faceTop.vertexPositions[i] = topFace[i] + offsetVec;
                faceTop.textureCoordinates[i] = textureCoords[i];

                faceBottom.vertexPositions[i] = bottomFace[i] + offsetVec;
                faceBottom.textureCoordinates[i] = textureCoords[i];
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