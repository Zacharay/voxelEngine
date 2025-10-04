#include "Chunk.hpp"
#include <array>
#include <random>

#include "TextureManager.hpp"
#include "WorldGenerator.hpp"
#include "World.hpp"



//bottom right //bottom left //top left
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
    { glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
      glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f) },
    // Bottom face (Y-)
    { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
      glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f) }
}};

Chunk::Chunk(const int x,const int y,const int z,World *world):m_world(world),blocks{},
    m_chunkPositionX(x),
    m_chunkPositionY(y),
    m_chunkPositionZ(z)
{
        for(int i=0;i<Config::chunkSize;i++)
            for(int j=0;j<Config::chunkSize;j++)
                for(int k=0;k<Config::chunkSize;k++) {

                    blocks[i][j][k] = BlockType::Air;
                }
}
bool Chunk::isBlockSolid(int x, int y, int z) {
    if( x<0 || x >= Config::chunkSize||
        y<0 || y >= Config::chunkSize||
        z<0 || z >= Config::chunkSize  ) {

            glm::vec3 globalPos = convertToWorldCoordinates(glm::vec3(x,y,z));
            BlockType block = m_world->getBlockAt(globalPos);

            return block != BlockType::Air;
        }

    return blocks[z][y][x] != BlockType::Air;
}


inline uint8_t Chunk::calcAO(bool side1, bool side2, bool corner)
{
    if (side1 && side2)
        return 60;   // darkest (strong corner shadow)

    int occ = int(side1) + int(side2) + int(corner);
    switch (occ) {
        case 0: return 255; // no occlusion
        case 1: return 170; // slight shadow
        case 2: return 110; // deeper shadow
        case 3: return 60;  // almost blocked
    }
    return 255;
}
u_int8_t Chunk::computeCornerAo(FaceDirection faceDir,int corner,int x,int y,int z) {
    int d1x=0,d1y=0,d1z=0;
    int d2x=0,d2y=0,d2z=0;
    int d3x=0,d3y=0,d3z=0;


    //bottom-right 0
    //bottom-left 1
    //top-left 2
    //top-right 3
    switch (faceDir) {
        case FaceDirection::Top:
            switch (corner) {
                case 0:
                    d1x = 1, d1y = 1, d1z = 0;
                    d2x = 0, d2y = 1, d2z = 1;
                    d3x = 1, d3y = 1, d3z = 1;
                break;
                case 1:
                    d1x = -1, d1y = 1, d1z = 0;
                    d2x = 0, d2y = 1, d2z = 1;
                    d3x = -1, d3y = 1, d3z = 1;
                break;
                case 2:
                    d1x = -1, d1y = 1, d1z = 0;
                    d2x = 0, d2y = 1, d2z = -1;
                    d3x = -1, d3y = 1, d3z = -1;
                break;
                case 3:
                    d1x = 1, d1y = 1, d1z = 0;
                    d2x = 0, d2y = 1, d2z = -1;
                    d3x = 1, d3y = 1, d3z = -1;
                break;
            }
            break;

        case FaceDirection::Front:
            switch (corner) {
                case 0:
                    d1x = 1, d1y = 0, d1z = -1;
                    d2x = 0, d2y = -1, d2z = -1;
                    d3x = 1, d3y = -1, d3z = -1;
                break;
                case 1:
                    d1x = -1, d1y = 0, d1z = 1;
                    d2x = 0, d2y = -1, d2z =  1;
                    d3x = -1, d3y = -1, d3z = 1;
                break;
                case 2:
                    d1x = -1, d1y = 0, d1z = 1;
                    d2x = 0, d2y = 1, d2z = 1;
                    d3x = -1, d3y = 1, d3z = 1;
                break;
                case 3:
                    d1x = 1, d1y = 0, d1z = 1;
                    d2x = 0, d2y = 1, d2z = 1;
                    d3x = 1, d3y = 1, d3z = 1;
                break;
            }
        break;
        case FaceDirection::Back:
            switch (corner) {
                case 0:
                    d1x = 1, d1y = 0, d1z = -1;
                    d2x = 0, d2y = -1, d2z = -1;
                    d3x = 1, d3y = -1, d3z = -1;
                break;
                case 1:
                    d1x = -1, d1y = 0, d1z =  -1;
                    d2x = 0, d2y = -1, d2z =  -1;
                    d3x = -1, d3y = -1, d3z = -1;
                break;
                case 2:
                    d1x = -1, d1y = 0, d1z = -1;
                    d2x = 0, d2y = 1, d2z = -1;
                    d3x = -1, d3y = 1, d3z = -1;
                break;
                case 3:
                    d1x = 1, d1y = 0, d1z = -1;
                    d2x = 0, d2y = 1, d2z = -1;
                    d3x = 1, d3y = 1, d3z = -1;
                break;
            }
        break;
        case FaceDirection::Right:
            switch (corner) {
                case 0:
                    d1x = 1, d1y = 0, d1z = 1;
                    d2x = 1, d2y = -1, d2z = 0;
                    d3x = 1, d3y = -1, d3z = 1;
                break;
                case 1:
                    d1x = 1, d1y = 0, d1z =  -1;
                    d2x = 1, d2y = -1, d2z =  0;
                    d3x = 1, d3y = -1, d3z = -1;
                break;
                case 2:
                    d1x = 1, d1y = 0, d1z = -1;
                    d2x = 1, d2y = 1, d2z = 0;
                    d3x = 1, d3y = 1, d3z = 1;
                break;
                case 3:
                    d1x = 1, d1y = 0, d1z = 1;
                    d2x = 1, d2y = 1, d2z = 0;
                    d3x = 1, d3y = 1, d3z = 1;
                break;
            }
        break;
        case FaceDirection::Left:
            switch (corner) {
                case 0:
                    d1x = -1, d1y = 0, d1z = 1;
                    d2x = -1, d2y = -1, d2z = 0;
                    d3x = -1, d3y = -1, d3z = 1;
                break;
                case 1:
                    d1x = -1, d1y = 0, d1z =  -1;
                    d2x = -1, d2y = -1, d2z =  0;
                    d3x = -1, d3y = -1, d3z = -1;
                break;
                case 2:
                    d1x = -1, d1y = 0, d1z = -1;
                    d2x = -1, d2y = 1, d2z = 0;
                    d3x = -1, d3y = 1, d3z = 1;
                break;
                case 3:
                    d1x = -1, d1y = 0, d1z = 1;
                    d2x = -1, d2y = 1, d2z = 0;
                    d3x = -1, d3y = 1, d3z = 1;
                break;
            }
        break;
        default:
            break;
    }

    bool side1 = isBlockSolid(x + d1x, y + d1y, z + d1z);
    bool side2 = isBlockSolid(x + d2x, y + d2y, z + d2z);
    bool side3 = isBlockSolid(x + d3x, y + d3y, z + d3z);

    return calcAO(side1,side2,side3);

}

void Chunk::setBlock(BlockType block,int x,int y,int z) {
    blocks[z][y][x] = block;
}
BlockType Chunk::getBlock(int x, int y, int z) {
    return (BlockType)blocks[z][y][x];
}

inline glm::vec3 Chunk::convertToWorldCoordinates(const glm::vec3 &coordinates) {
    const glm::vec3 offsetVec = glm::vec3(
                   static_cast<float>(m_chunkPositionX) * Config::chunkSize ,
                   static_cast<float>(m_chunkPositionY) * Config::chunkSize ,
                   static_cast<float>(m_chunkPositionZ) * Config::chunkSize );


    return coordinates + offsetVec;

}
void Chunk::generateMesh(std::vector<Face>& mesh, Chunk* chunkNx, Chunk* chunkPx, Chunk* chunkNy, Chunk* chunkPy, Chunk* chunkNz, Chunk* chunkPz) {
    for (int z= 0; z < Config::chunkSize; z++) {
        for (int y = 0; y < Config::chunkSize; y++) {
            for (int x = 0; x < Config::chunkSize; x++) {

                if (blocks[z][y][x] == BlockType::Air) continue;


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

                //bottom-right 0
                //bottom-left 1
                //top-left 2
                //top-right 3
                constexpr int triangleOrder[] = {0,1,2, 2,3,0};
                for (int i = 0; i < 6; i++) {
                    if (shouldRenderFace[i]) {
                        Face face;
                        for (int j = 0; j < 6; j++) {
                            int cornerID = triangleOrder[j];
                            glm::vec3 vertexPos = convertToWorldCoordinates(faceVertices[i][j]+ glm::vec3(x,y,z));
                            face.vertices[j].position = vertexPos;
                            face.vertices[j].ao = computeCornerAo((FaceDirection)i,cornerID,x,y,z);

                        }
                        TextureManager::getTextureCoordinates(face.vertices,(BlockType)blocks[z][y][x],(FaceDirection)i);
                        mesh.push_back(face);
                    }
                }
            }
        }
    }
}


