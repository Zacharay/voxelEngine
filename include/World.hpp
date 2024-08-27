#pragma once
#include <memory>
#include <vector>
#include "Chunk.hpp"

class World {


public:
    std::vector<std::unique_ptr<Chunk>>chunks;
    World();
    ~World();

};