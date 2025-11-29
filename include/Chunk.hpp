#pragma once

#include <vector>
#include <array>
#include <cstdint>
#include "Config.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "WorldGenerator.hpp"

#include <sys/types.h>

enum class BlockType:unsigned char;
class World;
struct Face;


// aaax xxxx yyyy zzzz zxxx xx
struct Vertex {
    std::uint32_t packedData;
};
struct Face {
    Vertex vertices[6];
};


enum FaceDirection {
    Front,
    Back,
    Left,
    Right,
    Top,
    Bottom
};

static constexpr std::size_t CHUNK_VOLUME = Config::chunkSize * Config::chunkSize * Config::chunkSize;

class Chunk {

public:

    Chunk()=default;

    void init(int x,int y,int z, World& world);

    void setBlock( int x, int y, int z,BlockType block);
    void generateMesh(std::vector<Face>& solidMesh, std::vector<Face>& transparentMesh,
                      Chunk* chunkNx, Chunk* chunkPx, Chunk* chunkNy,
                      Chunk* chunkPy, Chunk* chunkNz, Chunk* chunkPz);




    [[nodiscard]] BlockType getBlock(int x, int y, int z) const;


    [[nodiscard]] bool isChunkEmpty() const { return m_isChunkEmpty; }


    [[nodiscard]] int getPosX() const { return m_chunkPositionX; }
    [[nodiscard]] int getPosY() const { return m_chunkPositionY; }
    [[nodiscard]] int getPosZ() const { return m_chunkPositionZ; }


private:


    float getBlockOcclusion(int x, int y, int z, Chunk* chunkNx, Chunk* chunkPx,
                            Chunk* chunkNy, Chunk* chunkPy, Chunk* chunkNz, Chunk* chunkPz);

    std::uint8_t computeCornerAo(FaceDirection faceDir, int corner, int x, int y, int z,
                                 Chunk* chunkNx, Chunk* chunkPx, Chunk* chunkNy,
                                 Chunk* chunkPy, Chunk* chunkNz, Chunk* chunkPz);

    std::uint8_t calcAO(float side1, float side2, float corner);
    static u_int32_t getPackedVertexData(int posX,int posY,int posZ,int texX,int texY,u_int8_t aO);
    static constexpr unsigned int index(int x, int y, int z) {
        return x + y * Config::chunkSize + z * Config::chunkSize * Config::chunkSize;
    }


    static bool isBlockTransparent(BlockType type) {
        return type == BlockType::Air || type == BlockType::Water;
    }

    std::array<BlockType, CHUNK_VOLUME> blocks{};
    World* m_world = nullptr;
    bool m_isChunkEmpty = true;

    int m_chunkPositionX = 0;
    int m_chunkPositionY = 0;
    int m_chunkPositionZ = 0;
};