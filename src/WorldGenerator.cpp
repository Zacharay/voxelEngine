#include "WorldGenerator.hpp"

#include <World.hpp>
#include <iostream>
#include "Config.hpp"


constexpr float TEMP_COLD = -0.4f; // Below this is potentially Tundra
constexpr float TEMP_HOT = 0.4f;   // Above this is potentially Desert
constexpr float HUMID_DRY = 1.0f;
// Between COLD and HOT is Temperate

constexpr int MOUNTAIN_HEIGHT_START = 200; // Y-level where mountains begin to form
constexpr int SNOW_LINE = 220;

void WorldGenerator::buildOakTree(ChunkColumn *chunkColumn,int x_local,int y_start,int z_local){
    const int TRUNK_HEIGHT = 5;

    //generate trunk
    for(int i=0;i<TRUNK_HEIGHT;i++) {
        int chunkY = (y_start+i)/Config::chunkSize;
        int y_local = (y_start+i)%Config::chunkSize;
        chunkColumn->getChunk(chunkY)->setBlock(BlockType::OAK_LOG,x_local,y_local ,z_local);
    }

    const int TREE_LEAVES_START_POSITION = 3;

    for(int y=0;y<2;y++) {
        int chunkY = (y_start+TREE_LEAVES_START_POSITION+y)/Config::chunkSize;
        int y_local = (y_start+TREE_LEAVES_START_POSITION+y)%Config::chunkSize;
        for(int x=x_local - 2 ; x<=x_local + 2 ; x++) {
            for(int z=z_local - 2 ; z<=z_local + 2 ; z++) {
                if(z==z_local && x==x_local)continue;

                if(x>=0&&z>=0&&x<Config::chunkSize &&z<Config::chunkSize)
                chunkColumn->getChunk(chunkY)->setBlock(BlockType::OAK_LEAVES,x,y_local,z);
            }
        }
    }

    int chunkY = (y_start+TREE_LEAVES_START_POSITION+2)/Config::chunkSize;
    int y_local = (y_start+TREE_LEAVES_START_POSITION+2)%Config::chunkSize;
    for(int x=x_local - 1 ; x<=x_local + 1 ; x++) {
        for(int z=z_local - 1 ; z<=z_local + 1 ; z++) {
            if(x>=0&&z>=0&&x<Config::chunkSize &&z<Config::chunkSize)
            chunkColumn->getChunk(chunkY)->setBlock(BlockType::OAK_LEAVES,x,y_local,z);
        }
    }

    int directions[ ]= {-1,0,1};
    chunkY = (y_start+TREE_LEAVES_START_POSITION+3)/Config::chunkSize;
    y_local = (y_start+TREE_LEAVES_START_POSITION+3)%Config::chunkSize;
    for (int d :directions) {
        if(x_local+d>=0&&z_local>=0&&x_local+d<Config::chunkSize &&z_local<Config::chunkSize)
        chunkColumn->getChunk(chunkY)->setBlock(BlockType::OAK_LEAVES,x_local+d,y_local,z_local);
        if(x_local>=0&&z_local+d>=0&&x_local<Config::chunkSize &&z_local+d<Config::chunkSize)
        chunkColumn->getChunk(chunkY)->setBlock(BlockType::OAK_LEAVES,x_local,y_local,z_local+d);
    }
}
void WorldGenerator::buildCactus(ChunkColumn *chunkColumn,int x_local,int y_start,int z_local) {
    const int CACTUS_HEIGHT = 4;

    for(int i=0;i<CACTUS_HEIGHT;i++) {
        int chunkY = (y_start+i)/Config::chunkSize;
        int y_local = (y_start+i)%Config::chunkSize;
        chunkColumn->getChunk(chunkY)->setBlock(BlockType::Cactus,x_local,y_local ,z_local);
    }
}
void WorldGenerator::buildSpruceTree(ChunkColumn *chunkColumn,int x_local,int y_start,int z_local) {
    const int TRUNK_HEIGHT = 8;
    //generate trunk
    for(int i=0;i<TRUNK_HEIGHT;i++) {
        int chunkY = (y_start+i)/Config::chunkSize;
        int y_local = (y_start+i)%Config::chunkSize;
        chunkColumn->getChunk(chunkY)->setBlock(BlockType::Spruce_Log,x_local,y_local ,z_local);
    }
}
BiomeType WorldGenerator::getBiomeType(float temperature,float humidity,int terrainHeight) {


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
void WorldGenerator::spawnNature(ChunkColumn* chunkColumn, float noiseVal, int localX, int terrainHeight, int localZ, BiomeType biome) {

    const float PLAINS_TREE_THRESHOLD =8.0f;
    const float DESERT_CACTUS_THRESHOLD = 8.5f;
    const float TUNDRA_TREE_THRESHOLD = 9.0f;


    if (terrainHeight <= Config::SEA_LEVEL) {
        return;
    }


    int surface_chunkY = terrainHeight / Config::chunkSize;
    int surface_y_local = terrainHeight % Config::chunkSize;


    if (surface_chunkY < 0 || surface_chunkY >= Config::chunkColumnHeight) {
        std::cerr << "Warning: Invalid surface chunk index in spawnNature." << std::endl;
        return;
    }


    BlockType surfaceBlock = chunkColumn->getChunk(surface_chunkY)->getBlock(localX, surface_y_local, localZ);


    switch (biome) {
        case BiomeType::Plains:

            if (noiseVal > PLAINS_TREE_THRESHOLD && surfaceBlock == BlockType::Grass) {
                WorldGenerator::buildOakTree(chunkColumn, localX, terrainHeight + 1, localZ);
            }
            break;
        case BiomeType::Desert:

            if (noiseVal > DESERT_CACTUS_THRESHOLD && surfaceBlock == BlockType::Sand) {
                WorldGenerator::buildCactus(chunkColumn, localX, terrainHeight + 1, localZ);
            }
            break;
        case BiomeType::Tundra:
            if (noiseVal > DESERT_CACTUS_THRESHOLD && surfaceBlock == BlockType::Snow) {
                WorldGenerator::buildSpruceTree(chunkColumn, localX, terrainHeight + 1, localZ);
            }
            break;
        default:
            break;
    }

}