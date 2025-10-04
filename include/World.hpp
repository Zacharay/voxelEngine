#pragma once

#include <FastNoiseLite.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Chunk.hpp"
#include "ChunkColumn.hpp"
#include "glm/vec2.hpp"

namespace std {
    template <>
    struct hash<glm::ivec2> {
        std::size_t operator()(const glm::ivec2& v) const noexcept {
            std::size_t h1 = std::hash<int>()(v.x);
            std::size_t h2 = std::hash<int>()(v.y);
            return h1 ^ (h2 << 1); // Adjusted for 2D vector
        }
    };
}

using ChunkMap = std::unordered_map<glm::ivec2, ChunkColumn>;

class World {
public:
    World();
    void loadChunk(int chunkPosX,int chunkPosZ);
    void regenerateMeshes();
    void unloadFarChunks(int playerChunkX,int playerChunkZ);
    const ChunkMap& getChunks() const;
    BlockType getBlockAt(glm::ivec3 worldPos);
private:
    FastNoiseLite m_noise;
    ChunkMap m_chunks;
    void setNeighbours();
    ChunkColumn* getChunkColumn(int chunkPosX,int chunkPosZ);

};