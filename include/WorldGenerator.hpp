#include <glm/vec3.hpp>
#pragma once
// Define block types
enum BlockType {
    Air,
    Water,
    Grass,
    Sand,
    Stone,
    Dirt,
    BlockTypeCount
};


namespace WorldGenerator {
    inline BlockType generateBlock(int blockHeight) {
        BlockType block;
        if(blockHeight<60) {
            block = BlockType::Stone;
        }
        else if(blockHeight > 60 && blockHeight < 75) {
            block= BlockType::Sand;
        }
        else {
            block = BlockType::Grass;
        }
        return block;
    }
}