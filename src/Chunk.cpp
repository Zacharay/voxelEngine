#include "Chunk.hpp"
#include <array>
#include <random>

#include "TextureManager.hpp"
#include "WorldGenerator.hpp"
#include "World.hpp"
#include <iostream>

namespace {

    using namespace glm;

    // Bottom-Right, Bottom-Left, Top-Left, Top-Right
    constexpr std::array<std::array<ivec3, 6>, 6> FACE_VERTICES = {{
        // Front (Z+)
        { ivec3(1, 0, 1), ivec3(0, 0, 1), ivec3(0, 1, 1), ivec3(0, 1, 1), ivec3(1, 1, 1), ivec3(1, 0, 1) },
        // Back (Z-)
        { ivec3(1, 0, 0), ivec3(0, 0, 0), ivec3(0, 1, 0), ivec3(0, 1, 0), ivec3(1, 1, 0), ivec3(1, 0, 0) },
        // Left (X-)
        { ivec3(0, 0, 1), ivec3(0, 0, 0), ivec3(0, 1, 0), ivec3(0, 1, 0), ivec3(0, 1, 1), ivec3(0, 0, 1) },
        // Right (X+)
        { ivec3(1, 0, 1), ivec3(1, 0, 0), ivec3(1, 1, 0), ivec3(1, 1, 0), ivec3(1, 1, 1), ivec3(1, 0, 1) },
        // Top (Y+)
        { ivec3(1, 1, 1), ivec3(0, 1, 1), ivec3(0, 1, 0), ivec3(0, 1, 0), ivec3(1, 1, 0), ivec3(1, 1, 1) },
        // Bottom (Y-)
        { ivec3(0, 0, 1), ivec3(1, 0, 1), ivec3(1, 0, 0), ivec3(1, 0, 0), ivec3(0, 0, 0), ivec3(0, 0, 1) }
    }};

    struct AoOffsets {
        ivec3 side1, side2, corner;
    };

    constexpr std::array<std::array<AoOffsets, 4>, 6> AO_LOOKUP_TABLE = {{
        // Front (Z+)
        {{
            { ivec3( 1, 0, 1), ivec3( 0,-1, 1), ivec3( 1,-1, 1) },
            { ivec3(-1, 0, 1), ivec3( 0,-1, 1), ivec3(-1,-1, 1) },
            { ivec3(-1, 0, 1), ivec3( 0, 1, 1), ivec3(-1, 1, 1) },
            { ivec3( 1, 0, 1), ivec3( 0, 1, 1), ivec3( 1, 1, 1) }
        }},
        // Back (Z-)
        {{
            { ivec3( 1, 0,-1), ivec3( 0,-1,-1), ivec3( 1,-1,-1) },
            { ivec3(-1, 0,-1), ivec3( 0,-1,-1), ivec3(-1,-1,-1) },
            { ivec3(-1, 0,-1), ivec3( 0, 1,-1), ivec3(-1, 1,-1) },
            { ivec3( 1, 0,-1), ivec3( 0, 1,-1), ivec3( 1, 1,-1) }
        }},
        // Left (X-)
        {{
            { ivec3(-1, 0, 1), ivec3(-1,-1, 0), ivec3(-1,-1, 1) },
            { ivec3(-1, 0,-1), ivec3(-1,-1, 0), ivec3(-1,-1,-1) },
            { ivec3(-1, 0,-1), ivec3(-1, 1, 0), ivec3(-1, 1,-1) },
            { ivec3(-1, 0, 1), ivec3(-1, 1, 0), ivec3(-1, 1, 1) }
        }},
        // Right (X+)
        {{
            { ivec3( 1, 0, 1), ivec3( 1,-1, 0), ivec3( 1,-1, 1) },
            { ivec3( 1, 0,-1), ivec3( 1,-1, 0), ivec3( 1,-1,-1) },
            { ivec3( 1, 0,-1), ivec3( 1, 1, 0), ivec3( 1, 1,-1) },
            { ivec3( 1, 0, 1), ivec3( 1, 1, 0), ivec3( 1, 1, 1) }
        }},
        // Top (Y+)
        {{
            { ivec3( 1, 1, 0), ivec3( 0, 1, 1), ivec3( 1, 1, 1) },
            { ivec3(-1, 1, 0), ivec3( 0, 1, 1), ivec3(-1, 1, 1) },
            { ivec3(-1, 1, 0), ivec3( 0, 1,-1), ivec3(-1, 1,-1) },
            { ivec3( 1, 1, 0), ivec3( 0, 1,-1), ivec3( 1, 1,-1) }
        }},
        // Bottom (Y-)
        {{
            { ivec3(-1,-1, 0), ivec3( 0,-1, 1), ivec3(-1,-1, 1) },
            { ivec3( 1,-1, 0), ivec3( 0,-1, 1), ivec3( 1,-1, 1) },
            { ivec3( 1,-1, 0), ivec3( 0,-1,-1), ivec3( 1,-1,-1) },
            { ivec3(-1,-1, 0), ivec3( 0,-1,-1), ivec3(-1,-1,-1) }
        }}
    }};

    [[nodiscard]] inline bool shouldRenderFace(BlockType current, BlockType neighbor) {
        if (current == BlockType::Water) {
            return neighbor == BlockType::Air;
        }
        return BlockUtils::isTransparent(neighbor);
    }

}

void Chunk::init(int x, int y, int z, World &world) {
    m_chunkPositionX = x;
    m_chunkPositionY = y;
    m_chunkPositionZ = z;
    m_world = &world;
}
inline void Chunk::addFaceGeometry(int i, int x, int y, int z, BlockType currentBlock,
                                   const ChunkNeighbors& chunkNeighbors,
                                   std::vector<Face>& solidMesh,
                                   std::vector<Face>& transparentMesh) const {

    //Bottom-Right 0
    //Bottom-Left 1
    //Top-Left 2
    //Top-Right 3
    constexpr int triangleOrder[] = {0, 1, 2, 2, 3, 0};
    Face face;

    for (int j = 0; j < 6; j++) {
        int cornerID = triangleOrder[j];

        int localX = static_cast<int>(FACE_VERTICES[i][j].x) + x;
        int localY = static_cast<int>(FACE_VERTICES[i][j].y) + y;
        int localZ = static_cast<int>(FACE_VERTICES[i][j].z) + z;

        int worldY = localY + m_chunkPositionY * Config::chunkSize;

        int texX, texY;
        TextureManager::getTexturePosition(texX, texY, currentBlock, (FaceDirection)i);

        const u_int8_t aoLevel = computeCornerAo(static_cast<FaceDirection>(i), cornerID, x, y, z, chunkNeighbors);

        face.vertices[j] = getPackedVertexData(localX, worldY, localZ, texX, texY, aoLevel);
    }

    if (currentBlock == BlockType::Water) {
        transparentMesh.push_back(face);
    } else {
        solidMesh.push_back(face);
    }
}

inline uint8_t Chunk::calcAO(float side1, float side2, float corner)const
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


float Chunk::getBlockOcclusion(int x, int y, int z, const ChunkNeighbors& chunkNeigbors)const {
    BlockType blockType = BlockType::Air;

    if (x >= 0 && x < Config::chunkSize &&
        y >= 0 && y < Config::chunkSize &&
        z >= 0 && z < Config::chunkSize)
    {
        blockType = this->getBlock(x, y, z);
    }

    else if ((x < 0 || x >= Config::chunkSize) + (y < 0 || y >= Config::chunkSize) + (z < 0 || z >= Config::chunkSize) == 1) {
        if (x < 0)                          blockType = chunkNeigbors.left ? chunkNeigbors.left ->getBlock(Config::chunkSize + x, y, z) : BlockType::Air;
        else if (x >= Config::chunkSize)    blockType = chunkNeigbors.right  ? chunkNeigbors.right ->getBlock(x - Config::chunkSize, y, z) : BlockType::Air;
        else if (y < 0)                     blockType = chunkNeigbors.bottom ? chunkNeigbors.bottom->getBlock(x, Config::chunkSize + y, z) : BlockType::Air;
        else if (y >= Config::chunkSize)    blockType = chunkNeigbors.top ? chunkNeigbors.top->getBlock(x, y - Config::chunkSize, z) : BlockType::Air;
        else if (z < 0)                     blockType = chunkNeigbors.back ? chunkNeigbors.back->getBlock(x, y, Config::chunkSize + z) : BlockType::Air;
        else if (z >= Config::chunkSize)    blockType = chunkNeigbors.front ? chunkNeigbors.front->getBlock(x, y, z - Config::chunkSize) : BlockType::Air;
    }
    else {
        blockType = BlockType::Air;
    }

    if (blockType == BlockType::Air) return 0.0f;
    if (blockType == BlockType::Water)return 0.4f;

    return 1.0f;

}
u_int8_t Chunk::computeCornerAo(FaceDirection faceDir,int corner,int x,int y,int z,const ChunkNeighbors& chunkNeighbors)const {


    const auto& offsets = AO_LOOKUP_TABLE[static_cast<int>(faceDir)][corner];

    float sideOneOcclusion = getBlockOcclusion(x + offsets.side1.x, y + offsets.side1.y, z + offsets.side1.z,chunkNeighbors);

    float sideTwoOcclusion = getBlockOcclusion(x + offsets.side2.x, y + offsets.side2.y, z + offsets.side2.z,chunkNeighbors);


    float cornerOcclusion = getBlockOcclusion(x + offsets.corner.x, y + offsets.corner.y, z + offsets.corner.z,chunkNeighbors);

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

void Chunk::generateMesh(std::vector<Face>& solidMesh,std::vector<Face>&transparentMesh, const ChunkNeighbors& chunkNeighbors)const {
    for (int z = 1; z < Config::chunkSize-1; z++) {
        for (int y = 1; y < Config::chunkSize-1; y++) {
            for (int x = 1; x < Config::chunkSize-1; x++) {

                BlockType currentBlock = getBlock(x, y, z);
                if (currentBlock == BlockType::Air) [[likely]]continue;

                const BlockType neighborTypes[] = {
                    getBlock(x, y, z + 1), // Front
                    getBlock(x, y, z - 1), // Back
                    getBlock(x - 1, y, z), // Left
                    getBlock(x + 1, y, z), // Right
                    getBlock(x, y + 1, z), // Top
                    getBlock(x, y - 1, z)  // Bottom
                };
                for (int i = 0; i < 6; i++) {
                    if (shouldRenderFace(currentBlock, neighborTypes[i])) {
                        addFaceGeometry(i, x, y, z, currentBlock, chunkNeighbors, solidMesh, transparentMesh);
                    }
                }


            }
        }
    }

    //we delegate checking edge blocks to another function in order to make core loop avoid branching mispredictions
    generateBorderMesh(solidMesh,transparentMesh,chunkNeighbors);
}
void Chunk::generateBorderMesh(std::vector<Face>& solidMesh,std::vector<Face>&transparentMesh, const ChunkNeighbors& chunkNeighbors)const {

    int S = Config::chunkSize;

    auto processBlock = [&](int x, int y, int z) __attribute__((always_inline)) {
        BlockType currentBlock = blocks[index(x, y, z)];

        if (currentBlock == BlockType::Air) return;

        BlockType neighborTypes[6];

        neighborTypes[Front]= (z + 1 < S) ? blocks[index(x, y, z + 1)] : (chunkNeighbors.front ? chunkNeighbors.front->getBlock(x, y, 0) : BlockType::Air);
        neighborTypes[Back]  = (z - 1 >= 0) ? blocks[index(x, y, z - 1)] : (chunkNeighbors.back ? chunkNeighbors.back->getBlock(x, y, S-1) : BlockType::Air);
        neighborTypes[Left]  = (x - 1 >= 0) ? blocks[index(x - 1, y, z )] : (chunkNeighbors.left ? chunkNeighbors.left->getBlock(S-1, y, z) : BlockType::Air);
        neighborTypes[Right]  = (x + 1 < S) ? blocks[index(x + 1, y, z )] : (chunkNeighbors.right ? chunkNeighbors.right->getBlock(0, y, z) : BlockType::Air);
        neighborTypes[Top]  = (y + 1  < S) ? blocks[index(x, y + 1, z )] : (chunkNeighbors.top ? chunkNeighbors.top->getBlock(x, 0, z) : BlockType::Air);
        neighborTypes[Bottom]  = (y - 1 >= 0) ? blocks[index(x, y - 1, z )] : (chunkNeighbors.bottom ? chunkNeighbors.bottom->getBlock(x, S-1, z) : BlockType::Air);


        for (int i = 0; i < 6; i++) {
            if (shouldRenderFace(currentBlock, neighborTypes[i])) {
                addFaceGeometry(i, x, y, z, currentBlock, chunkNeighbors, solidMesh, transparentMesh);
            }
        }
    };

    for (int y = 0; y < S; y++) {
        for (int x = 0; x < S; x++) {
            processBlock(x, y, 0);
            processBlock(x, y, S - 1);
        }
    }


    for (int z = 1; z < S - 1; z++) {
        for (int x = 0; x < S; x++) {
            processBlock(x, 0, z);
            processBlock(x, S - 1, z);
        }
    }

    for (int z = 1; z < S - 1; z++) {
        for (int y = 1; y < S - 1; y++) {
            processBlock(0, y, z);
            processBlock(S - 1, y, z);
        }
    }

}

