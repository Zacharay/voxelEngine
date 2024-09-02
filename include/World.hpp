#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include "Chunk.hpp"


namespace std {
    template <>
    struct hash<glm::vec<3, int, glm::packed_highp>> {
        std::size_t operator()(const glm::vec<3, int, glm::packed_highp>& v) const noexcept {
            std::size_t h1 = std::hash<int>()(v.x);
            std::size_t h2 = std::hash<int>()(v.y);
            std::size_t h3 = std::hash<int>()(v.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
};


using ChunkMap = std::unordered_map<glm::ivec3,Chunk>;

class World {
private:
    ChunkMap m_chunks;
    void setNeighbours();
public:

    World();
    ~World();
    const ChunkMap& getChunks() const;
};