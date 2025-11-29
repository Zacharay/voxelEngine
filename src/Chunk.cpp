#include "Chunk.hpp"
#include <array>
#include <random>

#include "TextureManager.hpp"
#include "WorldGenerator.hpp"
#include "World.hpp"
#include <iostream>

// winding order
//bottom right //bottom left //top left //top right
constexpr std::array<std::array<glm::ivec3, 6>, 6> faceVertices = {{
    // Front face (Z+) -> Z=1
    { glm::ivec3(1, 0, 1), glm::ivec3(0, 0, 1), glm::ivec3(0, 1, 1),
      glm::ivec3(0, 1, 1), glm::ivec3(1, 1, 1), glm::ivec3(1, 0, 1) },

    // Back face (Z-) -> Z=0
    { glm::ivec3(1, 0, 0), glm::ivec3(0, 0, 0), glm::ivec3(0, 1, 0),
      glm::ivec3(0, 1, 0), glm::ivec3(1, 1, 0), glm::ivec3(1, 0, 0) },

    // Left face (X-) -> X=0
    { glm::ivec3(0, 0, 1), glm::ivec3(0, 0, 0), glm::ivec3(0, 1, 0),
      glm::ivec3(0, 1, 0), glm::ivec3(0, 1, 1), glm::ivec3(0, 0, 1) },

    // Right face (X+) -> X=1
    { glm::ivec3(1, 0, 1), glm::ivec3(1, 0, 0), glm::ivec3(1, 1, 0),
      glm::ivec3(1, 1, 0), glm::ivec3(1, 1, 1), glm::ivec3(1, 0, 1) },

    // Top face (Y+) -> Y=1
    { glm::ivec3(1, 1, 1), glm::ivec3(0, 1, 1), glm::ivec3(0, 1, 0),
      glm::ivec3(0, 1, 0), glm::ivec3(1, 1, 0), glm::ivec3(1, 1, 1) },

    // Bottom face (Y-) -> Y=0
    { glm::ivec3(0, 0, 1), glm::ivec3(1, 0, 1), glm::ivec3(1, 0, 0),
      glm::ivec3(1, 0, 0), glm::ivec3(0, 0, 0), glm::ivec3(0, 0, 1) }
}};

///                     zzzz xxxx yyyy yyyy
/// 0000 0000 0000 0000 0000 0000 0000 0000
///

//AO helpers
struct AoOffsets {
    glm::ivec3 side1;
    glm::ivec3 side2;
    glm::ivec3 corner;
};
enum Corner : int {
    BOTTOM_RIGHT = 0,
    BOTTOM_LEFT  = 1,
    TOP_LEFT     = 2,
    TOP_RIGHT    = 3
};
static constexpr std::array<std::array<AoOffsets, 4>, 6> aoLookupTable = {{
    // Front (Z+)
    {{
        { glm::ivec3( 1, 0, 1), glm::ivec3( 0,-1, 1), glm::ivec3( 1,-1, 1) },  // BOTTOM_RIGHT
        { glm::ivec3(-1, 0, 1), glm::ivec3( 0,-1, 1), glm::ivec3(-1,-1, 1) },  // BOTTOM_LEFT
        { glm::ivec3(-1, 0, 1), glm::ivec3( 0, 1, 1), glm::ivec3(-1, 1, 1) },  // TOP_LEFT
        { glm::ivec3( 1, 0, 1), glm::ivec3( 0, 1, 1), glm::ivec3( 1, 1, 1) }   // TOP_RIGHT
    }},
    // Back (Z-)
    {{
        { glm::ivec3( 1, 0,-1), glm::ivec3( 0,-1,-1), glm::ivec3( 1,-1,-1) },  // BOTTOM_RIGHT
        { glm::ivec3(-1, 0,-1), glm::ivec3( 0,-1,-1), glm::ivec3(-1,-1,-1) },  // BOTTOM_LEFT
        { glm::ivec3(-1, 0,-1), glm::ivec3( 0, 1,-1), glm::ivec3(-1, 1,-1) },  // TOP_LEFT
        { glm::ivec3( 1, 0,-1), glm::ivec3( 0, 1,-1), glm::ivec3( 1, 1,-1) }   // TOP_RIGHT
    }},
    // Left (X-)
    {{
        { glm::ivec3(-1, 0, 1), glm::ivec3(-1,-1, 0), glm::ivec3(-1,-1, 1) },  // BOTTOM_RIGHT
        { glm::ivec3(-1, 0,-1), glm::ivec3(-1,-1, 0), glm::ivec3(-1,-1,-1) },  // BOTTOM_LEFT
        { glm::ivec3(-1, 0,-1), glm::ivec3(-1, 1, 0), glm::ivec3(-1, 1,-1) },  // TOP_LEFT
        { glm::ivec3(-1, 0, 1), glm::ivec3(-1, 1, 0), glm::ivec3(-1, 1, 1) }   // TOP_RIGHT
    }},
    // Right (X+)
    {{
        { glm::ivec3( 1, 0, 1), glm::ivec3( 1,-1, 0), glm::ivec3( 1,-1, 1) },  // BOTTOM_RIGHT
        { glm::ivec3( 1, 0,-1), glm::ivec3( 1,-1, 0), glm::ivec3( 1,-1,-1) },  // BOTTOM_LEFT
        { glm::ivec3( 1, 0,-1), glm::ivec3( 1, 1, 0), glm::ivec3( 1, 1,-1) },  // TOP_LEFT
        { glm::ivec3( 1, 0, 1), glm::ivec3( 1, 1, 0), glm::ivec3( 1, 1, 1) }   // TOP_RIGHT
    }},
    // Top (Y+)
    {{
        { glm::ivec3( 1, 1, 0), glm::ivec3( 0, 1, 1), glm::ivec3( 1, 1, 1) },  // BOTTOM_RIGHT
        { glm::ivec3(-1, 1, 0), glm::ivec3( 0, 1, 1), glm::ivec3(-1, 1, 1) },  // BOTTOM_LEFT
        { glm::ivec3(-1, 1, 0), glm::ivec3( 0, 1,-1), glm::ivec3(-1, 1,-1) },  // TOP_LEFT
        { glm::ivec3( 1, 1, 0), glm::ivec3( 0, 1,-1), glm::ivec3( 1, 1,-1) }   // TOP_RIGHT
    }},
    // Bottom (Y-)
    {{
        { glm::ivec3(-1,-1, 0), glm::ivec3( 0,-1, 1), glm::ivec3(-1,-1, 1) },  // BOTTOM_RIGHT
        { glm::ivec3( 1,-1, 0), glm::ivec3( 0,-1, 1), glm::ivec3( 1,-1, 1) },  // BOTTOM_LEFT
        { glm::ivec3( 1,-1, 0), glm::ivec3( 0,-1,-1), glm::ivec3( 1,-1,-1) },  // TOP_LEFT
        { glm::ivec3(-1,-1, 0), glm::ivec3( 0,-1,-1), glm::ivec3(-1,-1,-1) }   // TOP_RIGHT
    }}
}};


void Chunk::init(int x, int y, int z, World &world) {
    m_chunkPositionX = x;
    m_chunkPositionY = y;
    m_chunkPositionZ = z;
    m_world = &world;
}


inline uint8_t Chunk::calcAO(float side1, float side2, float corner)
{

    if (side1 > 0.9f && side2 > 0.9f) {
        return 0;
    }

    float totalOcclusion = side1 + side2 + corner;

    float normalizedOcclusion = totalOcclusion / 3.0f;


    float brightness = 1.0f - normalizedOcclusion;

    float aoValue = (brightness * 7.0f) + 0.5f;

    if (aoValue > 7.0f) aoValue = 7.0f;
    if (aoValue < 0.0f) aoValue = 0.0f;

    return static_cast<uint8_t>(aoValue);
}


float Chunk::getBlockOcclusion(int x, int y, int z, Chunk* chunkNx, Chunk* chunkPx, Chunk* chunkNy, Chunk* chunkPy, Chunk* chunkNz, Chunk* chunkPz) {

    BlockType blockType = BlockType::Air;

    if (x >= 0 && x < Config::chunkSize &&
        y >= 0 && y < Config::chunkSize &&
        z >= 0 && z < Config::chunkSize)
    {
        blockType = this->getBlock(x, y, z);
    }

    else if ((x < 0 || x >= Config::chunkSize) + (y < 0 || y >= Config::chunkSize) + (z < 0 || z >= Config::chunkSize) == 1) {
        if (x < 0)                          blockType = chunkNx ? chunkNx->getBlock(Config::chunkSize + x, y, z) : BlockType::Air;
        else if (x >= Config::chunkSize)    blockType = chunkPx ? chunkPx->getBlock(x - Config::chunkSize, y, z) : BlockType::Air;
        else if (y < 0)                     blockType = chunkNy ? chunkNy->getBlock(x, Config::chunkSize + y, z) : BlockType::Air;
        else if (y >= Config::chunkSize)    blockType = chunkPy ? chunkPy->getBlock(x, y - Config::chunkSize, z) : BlockType::Air;
        else if (z < 0)                     blockType = chunkNz ? chunkNz->getBlock(x, y, Config::chunkSize + z) : BlockType::Air;
        else if (z >= Config::chunkSize)    blockType = chunkPz ? chunkPz->getBlock(x, y, z - Config::chunkSize) : BlockType::Air;
    }
    else {
        blockType = BlockType::Air;
    }

    if (blockType == BlockType::Air) {
        return 0.0f;
    } else if (blockType == BlockType::Water) {
        return 0.4f;
    } else {
        return 1.0f;
    }
}
u_int8_t Chunk::computeCornerAo(FaceDirection faceDir,int corner,int x,int y,int z,Chunk* chunkNx, Chunk* chunkPx, Chunk* chunkNy, Chunk* chunkPy, Chunk* chunkNz, Chunk* chunkPz) {


    const auto& offsets = aoLookupTable[static_cast<int>(faceDir)][corner];

    float sideOneOcclusion = getBlockOcclusion(x + offsets.side1.x, y + offsets.side1.y, z + offsets.side1.z,
                                               chunkNx, chunkPx, chunkNy, chunkPy, chunkNz, chunkPz);

    float sideTwoOcclusion = getBlockOcclusion(x + offsets.side2.x, y + offsets.side2.y, z + offsets.side2.z,
                                               chunkNx, chunkPx, chunkNy, chunkPy, chunkNz, chunkPz);


    float cornerOcclusion = getBlockOcclusion(x + offsets.corner.x, y + offsets.corner.y, z + offsets.corner.z,
                                              chunkNx, chunkPx, chunkNy, chunkPy, chunkNz, chunkPz);

    return calcAO(sideOneOcclusion, sideTwoOcclusion, cornerOcclusion);

}

void Chunk::setBlock(int x,int y,int z,BlockType block) {
    if(x<0 || y<0 || z<0 || x>Config::chunkSize || y>Config::chunkSize || z>Config::chunkSize) {
        std::cerr<<"Error in setBlock()"<<std::endl;
        return;
    }

    if(block != BlockType::Air)m_isChunkEmpty = false;

    blocks[index(x,y,z)] = block;
}
BlockType Chunk::getBlock(int x, int y, int z)const {
    return blocks[index(x,y,z)];
}
u_int32_t Chunk::getPackedVertexData(const int posX,const int posY,const int posZ,const int texX,const int texY,const u_int8_t aO) {
    // 4. PACKING (32 bits total)
    std::uint32_t packedData = 0;

    // Y: 9 bits [0-8]
    packedData |= (posY & 0x1FF);

    // X: 5 bits [9-13]
    packedData |= (posX & 0x1F) << 9;

    // Z: 5 bits [14-18]
    packedData |= (posZ & 0x1F) << 14;

    // Texture X: 5 bits [19-23]
    packedData |= (texX & 0x1F) << 19;

    // Texture Y: 5 bits [24-28]
    packedData |= (texY & 0x1F) << 24;

    // AO: 3 bits [29-31]
    packedData |= (aO & 0x7) << 29;

    return packedData;

}

void Chunk::generateMesh(std::vector<Face>& solidMesh,std::vector<Face>&transparentMesh, Chunk* chunkNx, Chunk* chunkPx, Chunk* chunkNy, Chunk* chunkPy, Chunk* chunkNz, Chunk* chunkPz) {
    for (int z = 0; z < Config::chunkSize; z++) {
        for (int y = 0; y < Config::chunkSize; y++) {
            for (int x = 0; x < Config::chunkSize; x++) {

                BlockType currentBlockType = getBlock(x, y, z);
                if (currentBlockType == BlockType::Air) continue;

                bool shouldRenderFace[6] = { false };
                BlockType neighborTypes[6];


                // Front (Z+)
                if (z + 1 == Config::chunkSize) {
                    neighborTypes[Front] = (chunkPz != nullptr) ? chunkPz->getBlock(x, y, 0) : BlockType::Air;
                } else {
                    neighborTypes[Front] = getBlock(x, y, z + 1);
                }

                // Back (Z-)
                if (z == 0) {
                    neighborTypes[Back] = (chunkNz != nullptr) ? chunkNz->getBlock(x, y, Config::chunkSize - 1) : BlockType::Air;
                } else {
                    neighborTypes[Back] = getBlock(x, y, z - 1);
                }

                // Left (X-)
                if (x == 0) {
                    neighborTypes[Left] = (chunkNx != nullptr) ? chunkNx->getBlock(Config::chunkSize - 1, y, z) : BlockType::Air;
                } else {
                    neighborTypes[Left] = getBlock(x - 1, y, z);
                }

                // Right (X+)
                if (x + 1 == Config::chunkSize) {
                    neighborTypes[Right] = (chunkPx != nullptr) ? chunkPx->getBlock(0, y, z) : BlockType::Air;
                } else {
                    neighborTypes[Right] = getBlock(x + 1, y, z);
                }

                // Top (Y+)
                if (y + 1 == Config::chunkSize) {
                    neighborTypes[Top] = (chunkPy != nullptr) ? chunkPy->getBlock(x, 0, z) : BlockType::Air;
                } else {
                    neighborTypes[Top] = getBlock(x, y + 1, z);
                }

                // Bottom (Y-)
                if (y == 0) {
                    neighborTypes[Bottom] = (chunkNy != nullptr) ? chunkNy->getBlock(x, Config::chunkSize - 1, z) : BlockType::Air;
                } else {
                    neighborTypes[Bottom] = getBlock(x, y - 1, z);
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



                            int localX = static_cast<int>(faceVertices[i][j].x) + x;
                            int localY = static_cast<int>(faceVertices[i][j].y) + y ;
                            int localZ = static_cast<int>(faceVertices[i][j].z) + z;
                            int worldY = localY + m_chunkPositionY * Config::chunkSize;

                            int texX, texY;
                            TextureManager::getTexturePosition(texX, texY, currentBlockType, (FaceDirection)i);

                            const u_int8_t aoLevel = computeCornerAo(static_cast<FaceDirection>(i), cornerID, x, y, z,
                               chunkNx, chunkPx, chunkNy, chunkPy, chunkNz, chunkPz);

                            face.vertices[j].packedData = getPackedVertexData(localX,worldY,localZ,texX,texY,aoLevel);
                        }


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


