#pragma once

#include <memory>
#include <vector>
#include "Chunk.hpp"

class World {

    std::vector<Chunk>chunks;
public:

    World();
    ~World();
    const std::vector<Chunk>& getChunks() const;
};