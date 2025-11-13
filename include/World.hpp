#pragma once

#include <FastNoiseLite.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Chunk.hpp"
#include "ChunkColumn.hpp"
#include "glm/vec2.hpp"
#include <climits>

#include "ThreadPool.hpp"

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

using ChunkMap = std::unordered_map<glm::ivec2, std::unique_ptr<ChunkColumn>>;

class World {
public:
    World();
    void loadChunk(int chunkPosX,int chunkPosZ);
    void regenerateMeshes();
    void unloadFarChunks(int playerChunkX,int playerChunkZ);
    const ChunkMap& getChunks() const;

    float getTemperatureNoiseVal(int x,int y)const;
    float getHeightNoiseVal(int x,int y)const;

    float getTreeNoiseVal(int x, int y) const;

    float getHumidityNoiseVal(int x,int y)const;
    void processChunkMeshes();
    BlockType getBlockAt(glm::ivec3 worldPos);
private:
    std::unique_ptr<ThreadPool> m_threadPool;

    FastNoiseLite m_noise;
    FastNoiseLite m_treeNoise;

    FastNoiseLite m_temperatureNoise;
    FastNoiseLite m_humidityNoise;

    //cache for getChunkColumn to prevent map searching
    ChunkColumn* m_lastAccessedColumn = nullptr;
    glm::ivec2 m_lastAccessedPos = glm::ivec2(INT_MIN);

    std::mutex m_chunkMapMutex;

    ChunkMap m_chunks;
    void setNeighbours();
    ChunkColumn* getChunkColumn(int chunkPosX,int chunkPosZ);

};