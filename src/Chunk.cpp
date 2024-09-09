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




void Chunk::generateMesh(std::vector<Face>&mesh) {
    for(int z=0;z<Config::chunkSize;z++)
        for(int y=0;y<Config::chunkSize;y++) {
            for(int x=0;x<Config::chunkSize;x++) {

                if(blocks[z][y][x]==BlockType::Air)continue;



                glm::vec3 offsetVec = glm::vec3(
                static_cast<float>(m_chunkPositionX)*16.0f+x,
                static_cast<float>(m_chunkPositionY)*16.0f+y,
                static_cast<float>(m_chunkPositionZ)*16.0f+z);

                bool shouldRenderFace[6] = {false};

                if(z + 1 == Config::chunkSize) {
                    if(m_chunkPz == nullptr) {
                        shouldRenderFace[Front] = true;
                    }
                    else {
                        shouldRenderFace[Front] = m_chunkPz->blocks[0][y][x]==BlockType::Air;
                    }

                }
                else {
                    if(blocks[z+1][y][x] == 0)
                        shouldRenderFace[Front]=true;
                }

                if(z==0) {
                    if(m_chunkNz == nullptr) {
                        shouldRenderFace[Back] = true;
                    }
                    else {
                        shouldRenderFace[Back] = m_chunkNz->blocks[Config::chunkSize-2][y][x]==BlockType::Air;
                    }

                }
                else {
                    if(blocks[z-1][y][x] == 0) {
                        shouldRenderFace[Back] = true;
                    }
                }


                if(x==0) {

                    if(m_chunkNx == nullptr) {
                        shouldRenderFace[Left] = true;
                    }
                    else {
                        shouldRenderFace[Left] = m_chunkNx->blocks[z][y][Config::chunkSize-2]==BlockType::Air;
                    }

                }
                else {
                    if(blocks[z][y][x - 1] == 0)
                        shouldRenderFace[Left] = true;
                }


                if(x + 1 == Config::chunkSize) {
                    if(m_chunkPx == nullptr) {
                        shouldRenderFace[Right] = true;
                    }
                    else {
                        shouldRenderFace[Right] = m_chunkPx->blocks[z][y][0]==BlockType::Air;
                    }

                }
                else {
                    if(blocks[z][y][x + 1] == 0)
                        shouldRenderFace[Right]=true;
                }


                if(y + 1 == Config::chunkSize) {
                    if(m_chunkPy == nullptr) {
                        shouldRenderFace[Top] = true;
                    }
                    else {
                        shouldRenderFace[Top] = m_chunkPy->blocks[z][0][x]==BlockType::Air;
                    }
                }
                else {
                    if(blocks[z][y+1][x] == 0)
                        shouldRenderFace[Top]=true;
                }


                if(y==0) {
                    if(m_chunkNy == nullptr) {
                        shouldRenderFace[Bottom] = true;
                    }
                    else {
                        shouldRenderFace[Bottom] = m_chunkNy->blocks[z][Config::chunkSize-2][x]==BlockType::Air;
                    }

                }
                else {
                    if(blocks[z][y-1][x] == 0) {
                        shouldRenderFace[Bottom] = true;
                    }
                }


                glm::vec3 color = blockColors[blocks[z][y][x]] ;
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
void Chunk::setNeighboursChunks(Chunk *chunkNx,Chunk *chunkPx,Chunk* chunkNy,Chunk *chunkPy,Chunk *chunkNz,Chunk *chunkPz) {
    m_chunkNx = chunkNx;
    m_chunkPx = chunkPx;
    m_chunkNy = chunkNy;
    m_chunkPy = chunkPy;
    m_chunkNz = chunkNz;
    m_chunkPz = chunkPz;
}
