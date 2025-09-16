#include <glm/vec3.hpp>
#pragma once
// Define block types
enum BlockType {
    Air,
    Grass,
    Sand,
    Stone,
    Dirt,
    BlockTypeCount
};


constexpr std::array<glm::vec3, BlockTypeCount> blockColors = {{
    glm::vec3(0.0f, 0.0f, 0.0f),   // Air - black or transparent
    glm::vec3(0.13f, 0.54f, 0.13f), // Grass - green
    glm::vec3(0.93f, 0.79f, 0.52f), // Sand - sandy color
    glm::vec3(0.6f, 0.65f, 0.69f),  // Stone - gray
    glm::vec3(0.58f,0.29f,0.0f)
}};

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