#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include "Chunk.hpp"
#include "ChunkColumn.hpp"

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
private:
    ChunkMap m_chunks;
    //void setNeighbours();
public:

    World();
    ~World();
    const ChunkMap& getChunks() const;
};