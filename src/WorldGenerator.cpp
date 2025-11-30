#include "WorldGenerator.hpp"

#include <World.hpp>
#include "Config.hpp"


constexpr float TEMP_COLD = -0.4f; // Below this is potentially Tundra
constexpr float TEMP_HOT = 0.4f;   // Above this is potentially Desert
constexpr float HUMID_DRY = 1.0f;


constexpr int MOUNTAIN_HEIGHT_START = 200; // Y-level where mountains begin to form
constexpr int SNOW_LINE = 220;

void WorldGenerator::buildOakTree(ChunkColumn& chunkColumn,int localX,int yStart,int localZ){
    const int TRUNK_HEIGHT = 5;

    //generate trunk
    for(int i=0;i<TRUNK_HEIGHT;i++) {
        chunkColumn.setBlockAt(localX,yStart+i,localZ,BlockType::OAK_LOG);
    }

    const int TREE_LEAVES_START_POSITION = 3;

    for(int i=0;i<2;i++) {
        const int worldY = yStart+TREE_LEAVES_START_POSITION+i;
        for(int z=localZ - 2 ; z<=localZ + 2 ; z++) {
            for(int x=localX - 2 ; x<=localX + 2 ; x++) {

                //skip trunk
                if(z==localZ && x==localX)continue;

                if(x>=0&&z>=0&&x<Config::chunkSize &&z<Config::chunkSize)
                chunkColumn.setBlockAt(x,worldY,z,BlockType::OAK_LEAVES);
            }
        }
    }

    for(int z=localZ - 1 ; z<=localZ + 1 ; z++) {
    for(int x=localX - 1 ; x<=localX + 1 ; x++) {

            const int worldY = yStart+TREE_LEAVES_START_POSITION+2;
            if(x>=0&&z>=0&&x<Config::chunkSize &&z<Config::chunkSize) {
                chunkColumn.setBlockAt(x,worldY,z,BlockType::OAK_LEAVES);
            }
        }
    }

    int directions[ ]= {-1,0,1};
    for (int d :directions) {
        const int worldY = yStart+TREE_LEAVES_START_POSITION+3;

        if(localX+d>=0&&localZ>=0&&localX+d<Config::chunkSize &&localZ<Config::chunkSize) {
            chunkColumn.setBlockAt(localX+d,worldY,localZ,BlockType::OAK_LEAVES);
        }
        if(localX>=0&&localZ+d>=0&&localX<Config::chunkSize &&localZ+d<Config::chunkSize) {
            chunkColumn.setBlockAt(localX,worldY,localZ+d,BlockType::OAK_LEAVES);
        }

    }
}
void WorldGenerator::buildCactus(ChunkColumn& chunkColumn,int localX,int yStart,int localZ) {
    const int CACTUS_HEIGHT = 4;

    for(int i=0;i<CACTUS_HEIGHT;i++) {

        chunkColumn.setBlockAt(localX,yStart+i,localZ,BlockType::Cactus);
    }
}
void WorldGenerator::buildSpruceTree(ChunkColumn& chunkColumn,int localX,int yStart,int localZ) {
    const int TRUNK_HEIGHT = 16;

    for(int i=0;i<TRUNK_HEIGHT;i++) {
        chunkColumn.setBlockAt(localX,yStart+i,localZ,BlockType::Spruce_Log);
    }
}
BiomeType WorldGenerator::getBiomeType(const float temperature,const float humidity,const int terrainHeight) {

    if (terrainHeight > SNOW_LINE) {

        if (temperature < TEMP_HOT + 0.2f) {
            return BiomeType::Tundra;
        } else {

            return BiomeType::Mountains;
        }
    }
    if (terrainHeight > MOUNTAIN_HEIGHT_START) {

        if (temperature < TEMP_COLD) {
            return BiomeType::Tundra;
        } else {
            return BiomeType::Mountains;
        }
    }


    if (temperature < TEMP_COLD) {
        return BiomeType::Tundra;
    } else if (temperature > TEMP_HOT) {

        if (humidity < HUMID_DRY) {
            return BiomeType::Desert;
        } else {
            return BiomeType::Plains;
        }
    } else {

        return BiomeType::Plains;
    }
}
void WorldGenerator::spawnNature(ChunkColumn& chunkColumn, float noiseVal, int localX, int terrainHeight, int localZ, BiomeType biome) {

    const float PLAINS_TREE_THRESHOLD =8.0f;
    const float DESERT_CACTUS_THRESHOLD = 8.0f;
    const float TUNDRA_TREE_THRESHOLD = 9.5f;


    if (terrainHeight <= Config::SEA_LEVEL) {
        return;
    }

    const BlockType surfaceBlock = chunkColumn.getBlockAt(localX,terrainHeight,localZ);

    switch (biome) {
        case BiomeType::Plains:

            if (noiseVal > PLAINS_TREE_THRESHOLD && surfaceBlock == BlockType::Grass) {
                buildOakTree(chunkColumn, localX, terrainHeight + 1, localZ);
            }
            break;
        case BiomeType::Desert:

            if (noiseVal > DESERT_CACTUS_THRESHOLD && surfaceBlock == BlockType::Sand) {
                buildCactus(chunkColumn, localX, terrainHeight + 1, localZ);
            }
            break;
        case BiomeType::Tundra:
            if (noiseVal > TUNDRA_TREE_THRESHOLD && surfaceBlock == BlockType::Snow) {
                buildSpruceTree(chunkColumn, localX, terrainHeight + 1, localZ);
            }
            break;
        default:
            break;
    }

}