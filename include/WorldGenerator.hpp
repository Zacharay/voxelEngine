#pragma once

#include <glm/vec3.hpp>

#include "Config.hpp"
class ChunkColumn;

// Define block types
enum class BlockType :unsigned char{
    Air,
    Water,
    Grass,
    Sand,
    Stone,
    Dirt,
    OAK_LOG,
    OAK_LEAVES,
    BlockTypeCount
};


namespace WorldGenerator {
    inline BlockType generateBlock(int blockHeight) {
        BlockType block;
        if(blockHeight < Config::SEA_LEVEL + 5 && blockHeight > Config::SEA_LEVEL - 40) {
            block = BlockType::Sand;
        }
        else if (blockHeight <= Config::SEA_LEVEL - 40) {
            block = BlockType::Stone;
        }
        else {
            block = BlockType::Grass;
        }
        return block;
    }

    void buildTree (ChunkColumn *chunkColumn,int x_local,int y_start,int z_local);

}