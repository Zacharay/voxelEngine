#include "Chunk.hpp"
#include <array>
#include <random>

#include "TextureManager.hpp"
#include "WorldGenerator.hpp"
#include "World.hpp"
#include <iostream>


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

Chunk::Chunk(const int x,const int y,const int z,World* world):blocks{},
    m_chunkPositionX(x),
    m_chunkPositionY(y),
    m_chunkPositionZ(z)
{
        m_world = world;

        for(int i=0;i<Config::chunkSize;i++)
            for(int j=0;j<Config::chunkSize;j++)
                for(int k=0;k<Config::chunkSize;k++) {

                    blocks[index(i,j,k)] = BlockType::Air;
                }
}


float Chunk::getBlockOcclusion(int x, int y, int z, Chunk* chunkNx, Chunk* chunkPx, Chunk* chunkNy, Chunk* chunkPy, Chunk* chunkNz, Chunk* chunkPz) {

    BlockType blockType;

    // Block inside this chunk
    if (x >= 0 && x < Config::chunkSize &&
        y >= 0 && y < Config::chunkSize &&
        z >= 0 && z < Config::chunkSize)
    {
        blockType = this->getBlock(x, y, z);
    }
    // Block outside chunk but not corner/edge (check direct neighbors)
    else if ((x < 0 || x >= Config::chunkSize) + (y < 0 || y >= Config::chunkSize) + (z < 0 || z >= Config::chunkSize) == 1) {
        if (x < 0)                blockType = chunkNx ? chunkNx->getBlock(Config::chunkSize + x, y, z) : BlockType::Air;
        else if (x >= Config::chunkSize) blockType = chunkPx ? chunkPx->getBlock(x - Config::chunkSize, y, z) : BlockType::Air;
        else if (y < 0)                blockType = chunkNy ? chunkNy->getBlock(x, Config::chunkSize + y, z) : BlockType::Air;
        else if (y >= Config::chunkSize) blockType = chunkPy ? chunkPy->getBlock(x, y - Config::chunkSize, z) : BlockType::Air;
        else if (z < 0)                blockType = chunkNz ? chunkNz->getBlock(x, y, Config::chunkSize + z) : BlockType::Air;
        else /* (z >= Config::chunkSize)*/ blockType = chunkPz ? chunkPz->getBlock(x, y, z - Config::chunkSize) : BlockType::Air;
    }
    // Block outside chunk on an edge or corner
    else {
        glm::vec3 globalPos = convertToWorldCoordinates(glm::vec3(x, y, z));

        blockType = m_world->getBlockAt(globalPos);
    }


    if (blockType == BlockType::Air) {
        return 0.0f;
    } else if (blockType == BlockType::Water) {
        return 0.4f;
    } else {
        return 1.0f;
    }

}


inline uint8_t Chunk::calcAO(float side1, float side2, float corner)
{
    const float MAX_AO = 255.0f;
    const float AO_SCALE = 195.0f;
    const int MIN_AO = 60;

    if (side1 > 0.9f && side2 > 0.9f) {
        return MIN_AO;
    }

    float totalOcclusion = side1 + side2 + corner;


    float aoFactor = totalOcclusion / 3.0f; // Normalize occlusion to 0.0 - 1.0
    uint8_t aoValue = static_cast<uint8_t>(MAX_AO - aoFactor * AO_SCALE); // Map 0->255, 1->60

    return aoValue;


}
u_int8_t Chunk::computeCornerAo(FaceDirection faceDir,int corner,int x,int y,int z,Chunk* chunkNx, Chunk* chunkPx, Chunk* chunkNy, Chunk* chunkPy, Chunk* chunkNz, Chunk* chunkPz) {
    int d1x=0,d1y=0,d1z=0;
    int d2x=0,d2y=0,d2z=0;
    int d3x=0,d3y=0,d3z=0;


    //bottom-right 0
    //bottom-left 1
    //top-left 2
    //top-right 3

    enum CornerDirection {
        BOTTOM_RIGHT,
        BOTTOM_LEFT,
        TOP_LEFT,
        TOP_RIGHT
    };

    switch (faceDir) {
        case FaceDirection::Top:
            switch (corner) {
                case BOTTOM_RIGHT:
                    d1x = 1, d1y = 1, d1z = 0;
                    d2x = 0, d2y = 1, d2z = 1;
                    d3x = 1, d3y = 1, d3z = 1;
                break;
                case BOTTOM_LEFT:
                    d1x = -1, d1y = 1, d1z = 0;
                    d2x = 0, d2y = 1, d2z = 1;
                    d3x = -1, d3y = 1, d3z = 1;
                break;
                case TOP_LEFT:
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
        case FaceDirection::Bottom:
            switch (corner) {
                case BOTTOM_RIGHT:
                    d1x = -1, d1y = -1, d1z =  0;
                d2x =  0, d2y = -1, d2z =  1;
                d3x = -1, d3y = -1, d3z =  1;
                break;
                case BOTTOM_LEFT:
                    d1x =  1, d1y = -1, d1z =  0;
                d2x =  0, d2y = -1, d2z =  1;
                d3x =  1, d3y = -1, d3z =  1;
                break;
                case TOP_LEFT:
                    d1x =  1, d1y = -1, d1z =  0;
                d2x =  0, d2y = -1, d2z = -1;
                d3x =  1, d3y = -1, d3z = -1;
                break;
                case TOP_RIGHT:
                    d1x = -1, d1y = -1, d1z =  0;
                d2x =  0, d2y = -1, d2z = -1;
                d3x = -1, d3y = -1, d3z = -1;
                break;
            }
        break;
        case FaceDirection::Front:
            switch (corner) {
                case BOTTOM_RIGHT:
                    d1x = 1, d1y = 0, d1z = 1;
                    d2x = 0, d2y = -1, d2z = 1;
                    d3x = 1, d3y = -1, d3z = 1;
                break;
                case BOTTOM_LEFT:
                    d1x = -1, d1y = 0, d1z = 1;
                    d2x = 0, d2y = -1, d2z =  1;
                    d3x = -1, d3y = -1, d3z = 1;
                break;
                case TOP_LEFT:
                    d1x = -1, d1y = 0, d1z = 1;
                    d2x = 0, d2y = 1, d2z = 1;
                    d3x = -1, d3y = 1, d3z = 1;
                break;
                case TOP_RIGHT:
                    d1x = 1, d1y = 0, d1z = 1;
                    d2x = 0, d2y = 1, d2z = 1;
                    d3x = 1, d3y = 1, d3z = 1;
                break;
            }
        break;
        case FaceDirection::Back:
            switch (corner) {
                case BOTTOM_RIGHT:
                    d1x = 1, d1y = 0, d1z = -1;
                    d2x = 0, d2y = -1, d2z = -1;
                    d3x = 1, d3y = -1, d3z = -1;
                break;
                case BOTTOM_LEFT:
                    d1x = -1, d1y = 0, d1z =  -1;
                    d2x = 0, d2y = -1, d2z =  -1;
                    d3x = -1, d3y = -1, d3z = -1;
                break;
                case TOP_LEFT:
                    d1x = -1, d1y = 0, d1z = -1;
                    d2x = 0, d2y = 1, d2z = -1;
                    d3x = -1, d3y = 1, d3z = -1;
                break;
                case TOP_RIGHT:
                    d1x = 1, d1y = 0, d1z = -1;
                    d2x = 0, d2y = 1, d2z = -1;
                    d3x = 1, d3y = 1, d3z = -1;
                break;
            }
        break;
        case FaceDirection::Right:
            switch (corner) {
                case BOTTOM_RIGHT:
                    d1x = 1, d1y = 0, d1z = 1;
                    d2x = 1, d2y = -1, d2z = 0;
                    d3x = 1, d3y = -1, d3z = 1;
                break;
                case BOTTOM_LEFT:
                    d1x = 1, d1y = 0, d1z =  -1;
                    d2x = 1, d2y = -1, d2z =  0;
                    d3x = 1, d3y = -1, d3z = -1;
                break;
                case TOP_LEFT:
                    d1x = 1, d1y = 0, d1z = -1;
                    d2x = 1, d2y = 1, d2z = 0;
                    d3x = 1, d3y = 1, d3z = 1;
                break;
                case TOP_RIGHT:
                    d1x = 1, d1y = 0, d1z = 1;
                    d2x = 1, d2y = 1, d2z = 0;
                    d3x = 1, d3y = 1, d3z = 1;
                break;
            }
        break;
        case FaceDirection::Left:
            switch (corner) {
                case BOTTOM_RIGHT:
                    d1x = -1, d1y = 0, d1z = 1;
                    d2x = -1, d2y = -1, d2z = 0;
                    d3x = -1, d3y = -1, d3z = 1;
                break;
                case BOTTOM_LEFT:
                    d1x = -1, d1y = 0, d1z =  -1;
                    d2x = -1, d2y = -1, d2z =  0;
                    d3x = -1, d3y = -1, d3z = -1;
                break;
                case TOP_LEFT:
                    d1x = -1, d1y = 0, d1z = -1;
                    d2x = -1, d2y = 1, d2z = 0;
                    d3x = -1, d3y = 1, d3z = 1;
                break;
                case TOP_RIGHT:
                    d1x = -1, d1y = 0, d1z = 1;
                    d2x = -1, d2y = 1, d2z = 0;
                    d3x = -1, d3y = 1, d3z = 1;
                break;
            }
        break;
        default:
            break;
    }

    float sideOneOcclusion = getBlockOcclusion(x + d1x, y + d1y, z + d1z,chunkNx, chunkPx, chunkNy, chunkPy, chunkNz, chunkPz);
    float sideTwoOcclusion = getBlockOcclusion(x + d2x, y + d2y, z + d2z,chunkNx, chunkPx, chunkNy, chunkPy, chunkNz, chunkPz);
    float cornerOcclusion= getBlockOcclusion(x + d3x, y + d3y, z + d3z,chunkNx, chunkPx, chunkNy, chunkPy, chunkNz, chunkPz);

    return calcAO(sideOneOcclusion,sideTwoOcclusion,cornerOcclusion);

}

void Chunk::setBlock(BlockType block,int x,int y,int z) {
    if(x<0 || y<0 || z<0 || x>Config::chunkSize || y>Config::chunkSize || z>Config::chunkSize) {
        std::cerr<<"Error in setBlock()"<<std::endl;
        return;
    }


    blocks[index(x,y,z)] = block;
}
BlockType Chunk::getBlock(int x, int y, int z) {
    return (BlockType)blocks[index(x,y,z)];
}

inline glm::vec3 Chunk::convertToWorldCoordinates(const glm::vec3 &coordinates) {
    const glm::vec3 offsetVec = glm::vec3(
                   static_cast<float>(m_chunkPositionX) * Config::chunkSize ,
                   static_cast<float>(m_chunkPositionY) * Config::chunkSize ,
                   static_cast<float>(m_chunkPositionZ) * Config::chunkSize );


    return coordinates + offsetVec;

}
inline bool Chunk::isBlockTransparent(BlockType type) {
    return type == BlockType::Air || type == BlockType::Water;
}
void Chunk::generateMesh(std::vector<Face>& solidMesh,std::vector<Face>&transparentMesh, Chunk* chunkNx, Chunk* chunkPx, Chunk* chunkNy, Chunk* chunkPy, Chunk* chunkNz, Chunk* chunkPz) {
   for (int z = 0; z < Config::chunkSize; z++) {
        for (int y = 0; y < Config::chunkSize; y++) {
            for (int x = 0; x < Config::chunkSize; x++) {

                BlockType currentBlockType = this->getBlock(x, y, z);
                if (currentBlockType == BlockType::Air) continue;

                bool shouldRenderFace[6] = { false };
                BlockType neighborTypes[6];


                // Front (Z+)
                if (z + 1 == Config::chunkSize) {
                    neighborTypes[Front] = (chunkPz != nullptr) ? chunkPz->getBlock(x, y, 0) : BlockType::Air;
                } else {
                    neighborTypes[Front] = this->getBlock(x, y, z + 1);
                }

                // Back (Z-)
                if (z == 0) {
                    neighborTypes[Back] = (chunkNz != nullptr) ? chunkNz->getBlock(x, y, Config::chunkSize - 1) : BlockType::Air;
                } else {
                    neighborTypes[Back] = this->getBlock(x, y, z - 1);
                }

                // Left (X-)
                if (x == 0) {
                    neighborTypes[Left] = (chunkNx != nullptr) ? chunkNx->getBlock(Config::chunkSize - 1, y, z) : BlockType::Air;
                } else {
                    neighborTypes[Left] = this->getBlock(x - 1, y, z);
                }

                // Right (X+)
                if (x + 1 == Config::chunkSize) {
                    neighborTypes[Right] = (chunkPx != nullptr) ? chunkPx->getBlock(0, y, z) : BlockType::Air;
                } else {
                    neighborTypes[Right] = this->getBlock(x + 1, y, z);
                }

                // Top (Y+)
                if (y + 1 == Config::chunkSize) {
                    neighborTypes[Top] = (chunkPy != nullptr) ? chunkPy->getBlock(x, 0, z) : BlockType::Air;
                } else {
                    neighborTypes[Top] = this->getBlock(x, y + 1, z);
                }

                // Bottom (Y-)
                if (y == 0) {
                    neighborTypes[Bottom] = (chunkNy != nullptr) ? chunkNy->getBlock(x, Config::chunkSize - 1, z) : BlockType::Air;
                } else {
                    neighborTypes[Bottom] = this->getBlock(x, y - 1, z);
                }


                for (int i = 0; i < 6; i++) {
                    if (currentBlockType == BlockType::Water ) {

                        if (neighborTypes[i] == BlockType::Air) {
                            shouldRenderFace[i] = true;
                        }
                    } else {

                        if (isBlockTransparent(neighborTypes[i])) {
                            shouldRenderFace[i] = true;
                        }
                    }
                }

                //bottom-right 0
                //bottom-left 1
                //top-left 2
                //top-right 3
                constexpr int triangleOrder[] = {0, 1, 2, 2, 3, 0};
                for (int i = 0; i < 6; i++) {
                    if (shouldRenderFace[i]) {
                        Face face;
                        for (int j = 0; j < 6; j++) {
                            int cornerID = triangleOrder[j];
                            glm::vec3 vertexPos = convertToWorldCoordinates(faceVertices[i][j] + glm::vec3(x, y, z));
                            face.vertices[j].position = vertexPos;
                            face.vertices[j].ao = computeCornerAo((FaceDirection)i, cornerID, x, y, z,
                                chunkNx, chunkPx, chunkNy, chunkPy, chunkNz, chunkPz);
                        }
                        TextureManager::getTextureCoordinates(face.vertices, currentBlockType, (FaceDirection)i);

                        if (currentBlockType == BlockType::Water ) {
                            transparentMesh.push_back(face);
                        } else {
                            solidMesh.push_back(face);
                        }
                    }
                }
            }
        }
    }
}


