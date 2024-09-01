#include "Chunk.hpp"
#include <iostream>
#include <array>





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
    if(block==Air) {
        for(int j=0;j<Config::chunkSize;j++)
        for(int i=j;i<Config::chunkSize;i++)
                for(int k=j;k<Config::chunkSize;k++) {

                    blocks[i][j][k] = Grass;
                }
    }
    else {
        for(int i=0;i<Config::chunkSize;i++)
            for(int j=0;j<Config::chunkSize;j++)
                for(int k=0;k<Config::chunkSize;k++) {
                    int a  = (i+j+k)%3+1;
                    BlockType b = (BlockType)a;
                    blocks[i][j][k] = b;
                }
    }


}

void Chunk::generateMesh() {
    for(int z=0;z<Config::chunkSize;z++)
        for(int y=0;y<Config::chunkSize;y++) {
            for(int x=0;x<Config::chunkSize;x++) {

                if(blocks[z][y][x]==BlockType::Air)continue;

                glm::vec3 offsetVec = glm::vec3(
                static_cast<float>(m_chunkPositionX)*16.0f+x,
                static_cast<float>(m_chunkPositionY)*16.0f+y,
                static_cast<float>(m_chunkPositionZ)*16.0f+z);

                std::array<bool, 6> shouldRenderFace = {{
                    (z + 1 == Config::chunkSize || blocks[z + 1][y][x] == 0), // FRONT
                    (z == 0 || blocks[z - 1][y][x] == 0),                     // BACK
                    (x == 0 || blocks[z][y][x - 1] == 0),                     // LEFT
                    (x + 1 == Config::chunkSize || blocks[z][y][x + 1] == 0), // RIGHT
                    (y + 1 == Config::chunkSize || blocks[z][y + 1][x] == 0), // TOP
                    (y == 0 || blocks[z][y - 1][x] == 0)                      // BOTTOM
                }};


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
const std::vector<Face> &Chunk::getMesh()const {


    return mesh;
}