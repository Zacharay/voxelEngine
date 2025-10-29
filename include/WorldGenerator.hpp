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
    Spruce_Log,
    Spruce_Leaves,
    Cactus,
    Snow,
    BlockTypeCount
};
enum class BiomeType :unsigned char{
    Ocean,      // Below sea level
    Plains,     // Default moderate temp, lower elevation
    Desert,     // Hot, lower elevation
    Tundra,       // Cold, any elevation OR Moderate temp, high elevation
    Mountains   // High elevation, not extremely cold (could be rocky/snowy)
};



namespace WorldGenerator {
    inline BlockType generateBlock(int blockHeight,BiomeType biome) {
        BlockType block;

        if(biome==BiomeType::Plains) {
            if(blockHeight < Config::SEA_LEVEL + 5 && blockHeight > Config::SEA_LEVEL - 40) {
                block = BlockType::Sand;
            }
            else if (blockHeight <= Config::SEA_LEVEL - 40) {
                block = BlockType::Stone;
            }
            else {
                block = BlockType::Grass;
            }

        }
        else if(biome==BiomeType::Tundra) {
            block = BlockType::Snow;
        }
        else if(biome==BiomeType::Mountains) {
            block = BlockType::Stone;
        }
        else if(biome==BiomeType::Desert) {
            block = BlockType::Sand;
        }

        return block;

    }
    BiomeType getBiomeType(float temperature,float humidity,int terrainHeight);
    void buildOakTree (ChunkColumn *chunkColumn,int x_local,int y_start,int z_local);
    void buildSpruceTree(ChunkColumn *chunkColumn,int x_local,int y_start,int z_local);
    void buildCactus(ChunkColumn *chunkColumn,int x_local,int y_start,int z_local);
    void spawnNature(ChunkColumn *chunkColumn,float noiseVal,int localX,int terrainHeight,int localZ,BiomeType biome);
}