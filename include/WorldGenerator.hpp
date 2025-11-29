#pragma once


#include "Config.hpp"
#include "BlockTypes.hpp"
class ChunkColumn;


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
    BiomeType getBiomeType( float temperature, float humidity, int terrainHeight);
    void buildOakTree (ChunkColumn& chunkColumn,int localX,int yStart,int localZ);
    void buildSpruceTree(ChunkColumn& chunkColumn,int localX,int yStart,int localZ);
    void buildCactus(ChunkColumn& chunkColumn,int localX,int yStart,int localZ);

    void spawnNature(ChunkColumn& chunkColumn,float noiseVal,int localX,int terrainHeight,int localZ,BiomeType biome);
}