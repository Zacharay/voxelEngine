#include "Chunk.hpp"
#include <iostream>
#include <array>


enum FaceDirection {
    Front,
    Back,
    Left,
    Right,
    Top,
    Bottom
};


constexpr std::array<std::array<glm::vec3, 6>, 6> faceVertices = {{
    // Front face (Z+)
    { glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
      glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f) },
    // Back face (Z-)
    { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
      glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f) },
    // Left face (X-)
    { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
      glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f) },
    // Right face (X+)
    { glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f),
      glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f) },
    // Top face (Y+)
    { glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, -0.5f),
      glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f) },
    // Bottom face (Y-)
    { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
      glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f) }
}};

Chunk::Chunk(const int x,const int y,const int z,BlockType block):blocks{},
    m_chunkPositionX(x),
    m_chunkPositionY(y),
    m_chunkPositionZ(z)
{
        for(int i=0;i<Config::chunkSize;i++)
            for(int j=0;j<Config::chunkSize;j++)
                for(int k=0;k<Config::chunkSize;k++) {
                    int b = (i+j)%3+1;
                    if(block==Air) {
                        blocks[i][j][k] = Air;
                        continue;
                    }
                    blocks[i][j][k] = (BlockType)b;
                }
}




void Chunk::generateMesh(std::vector<Face>& mesh, Chunk* chunkNx, Chunk* chunkPx, Chunk* chunkNy, Chunk* chunkPy, Chunk* chunkNz, Chunk* chunkPz) {
    for (int z = 0; z < Config::chunkSize; z++) {
        for (int y = 0; y < Config::chunkSize; y++) {
            for (int x = 0; x < Config::chunkSize; x++) {

                if (blocks[z][y][x] == BlockType::Air) continue;

                glm::vec3 offsetVec = glm::vec3(
                    static_cast<float>(m_chunkPositionX) * 16.0f + x,
                    static_cast<float>(m_chunkPositionY) * 16.0f + y,
                    static_cast<float>(m_chunkPositionZ) * 16.0f + z
                );

                bool shouldRenderFace[6] = { false };

                // Check front face (Z+)
                if (z + 1 == Config::chunkSize) {
                    if (chunkPz == nullptr) {
                        shouldRenderFace[Front] = true;
                    } else {
                        shouldRenderFace[Front] = chunkPz->blocks[0][y][x] == BlockType::Air;
                    }
                } else {
                    if (blocks[z + 1][y][x] == BlockType::Air) {
                        shouldRenderFace[Front] = true;
                    }
                }

                // Check back face (Z-)
                if (z == 0) {
                    if (chunkNz == nullptr) {
                        shouldRenderFace[Back] = true;
                    } else {
                        shouldRenderFace[Back] = chunkNz->blocks[Config::chunkSize - 1][y][x] == BlockType::Air;
                    }
                } else {
                    if (blocks[z - 1][y][x] == BlockType::Air) {
                        shouldRenderFace[Back] = true;
                    }
                }

                // Check left face (X-)
                if (x == 0) {
                    if (chunkNx == nullptr) {
                        shouldRenderFace[Left] = true;
                    } else {
                        shouldRenderFace[Left] = chunkNx->blocks[z][y][Config::chunkSize - 1] == BlockType::Air;
                    }
                } else {
                    if (blocks[z][y][x - 1] == BlockType::Air) {
                        shouldRenderFace[Left] = true;
                    }
                }

                // Check right face (X+)
                if (x + 1 == Config::chunkSize) {
                    if (chunkPx == nullptr) {
                        shouldRenderFace[Right] = true;
                    } else {
                        shouldRenderFace[Right] = chunkPx->blocks[z][y][0] == BlockType::Air;
                    }
                } else {
                    if (blocks[z][y][x + 1] == BlockType::Air) {
                        shouldRenderFace[Right] = true;
                    }
                }

                // Check top face (Y+)
                if (y + 1 == Config::chunkSize) {
                    if (chunkPy == nullptr) {
                        shouldRenderFace[Top] = true;
                    } else {
                        shouldRenderFace[Top] = chunkPy->blocks[z][0][x] == BlockType::Air;
                    }
                } else {
                    if (blocks[z][y + 1][x] == BlockType::Air) {
                        shouldRenderFace[Top] = true;
                    }
                }

                // Check bottom face (Y-)
                if (y == 0) {
                    if (chunkNy == nullptr) {
                        shouldRenderFace[Bottom] = true;
                    } else {
                        shouldRenderFace[Bottom] = chunkNy->blocks[z][Config::chunkSize - 1][x] == BlockType::Air;
                    }
                } else {
                    if (blocks[z][y - 1][x] == BlockType::Air) {
                        shouldRenderFace[Bottom] = true;
                    }
                }

                // Create faces for the visible sides
                glm::vec3 color = blockColors[blocks[z][y][x]];
                for (int i = 0; i < 6; i++) {
                    if (shouldRenderFace[i]) {
                        Face face;
                        for (int j = 0; j < 6; j++) {
                            glm::vec3 vertexPos = faceVertices[i][j] + offsetVec;
                            face.vertices[j].position = vertexPos;
                            face.vertices[j].color = color;
                        }
                        mesh.push_back(face);
                    }
                }
            }
        }
    }
}


