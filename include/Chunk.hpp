#pragma once

#include <vector>
#include <array>
#include <cstdint>
#include "Config.hpp"
#include "BlockTypes.hpp"

// Forward declarations
class World;
class Chunk;


struct ChunkNeighbors {
    Chunk* left   = nullptr; // X-
    Chunk* right  = nullptr; // X+
    Chunk* bottom = nullptr; // Y-
    Chunk* top    = nullptr; // Y+
    Chunk* back   = nullptr; // Z-
    Chunk* front  = nullptr; // Z+


    Chunk* operator[](int i) const {
        const Chunk* arr[] = {back, front, left, right, top, bottom};
        return const_cast<Chunk*>(arr[i]);
    }
};


// aaax xxxx yyyy zzzz zxxx xx
struct Face {
    std::uint32_t vertices[6];
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
    Chunk() = default;

    void init(int x, int y, int z, World& world);
    void setBlock(int x, int y, int z, BlockType block);

    void generateMesh(std::vector<Face>& solidMesh,
                      std::vector<Face>& transparentMesh,
                      const ChunkNeighbors& chunkNeighbors) const;

    [[nodiscard]] BlockType getBlock(int x, int y, int z) const;
    [[nodiscard]] bool isChunkEmpty() const { return m_isChunkEmpty; }

    [[nodiscard]] int getPosX() const { return m_chunkPositionX; }
    [[nodiscard]] int getPosY() const { return m_chunkPositionY; }
    [[nodiscard]] int getPosZ() const { return m_chunkPositionZ; }

private:
    //Mesh Gen Helpers
    void generateBorderMesh(std::vector<Face>& solid,
                            std::vector<Face>& trans,
                            const ChunkNeighbors& neighbors) const;

    inline void addFaceGeometry(int i, int x, int y, int z, BlockType currentBlock,
                                const ChunkNeighbors& chunkNeighbors,
                                std::vector<Face>& solidMesh,
                                std::vector<Face>& transparentMesh) const;

    // AO & Occlusion
    [[nodiscard]] float getBlockOcclusion(int x, int y, int z, const ChunkNeighbors& neighbors) const;
    [[nodiscard]] std::uint8_t computeCornerAo(FaceDirection faceDir, int corner, int x, int y, int z, const ChunkNeighbors& neighbors) const;
    [[nodiscard]] std::uint8_t calcAO(float side1, float side2, float corner) const;

    // Math & Packing
    struct PackedVertexBits {
        std::uint32_t y : 9;  // 0-511
        std::uint32_t x : 5;  // 0-31
        std::uint32_t z : 5;  // 0-31
        std::uint32_t u : 5;  // Texture U
        std::uint32_t v : 5;  // Texture V
        std::uint32_t ao : 3; // Ambient Occlusion
    };

    [[nodiscard]] static constexpr std::uint32_t getPackedVertexData(int x, int y, int z, int u, int v, std::uint8_t ao) {
        const PackedVertexBits bits {
            .y  = static_cast<std::uint32_t>(y),
            .x  = static_cast<std::uint32_t>(x),
            .z  = static_cast<std::uint32_t>(z),
            .u  = static_cast<std::uint32_t>(u),
            .v  = static_cast<std::uint32_t>(v),
            .ao = static_cast<std::uint32_t>(ao)
        };
        return std::bit_cast<std::uint32_t>(bits);
    }

    [[nodiscard]] static constexpr unsigned int index(int x, int y, int z) {
        return x + y * Config::chunkSize + z * Config::chunkSize * Config::chunkSize;
    }

    //Member Data

    std::array<BlockType, CHUNK_VOLUME> blocks{};
    World* m_world = nullptr;

    int m_chunkPositionX = 0;
    int m_chunkPositionY = 0;
    int m_chunkPositionZ = 0;

    bool m_isChunkEmpty = true;
};