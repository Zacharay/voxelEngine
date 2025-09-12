#include "World.hpp"
#include "FastNoiseLite.h"
#include <iostream>
#include <Config.hpp>
#include <cfloat>


World::World() {
    FastNoiseLite noise;

    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.0015f);

    // std::vector<std::vector<unsigned int>> noiseData(Config::noiseWidth, std::vector<unsigned int>(Config::noiseWidth, 0));
    //
    // for(int z=0;z< Config::noiseWidth;z++) {
    //     for(int x=0;x< Config::noiseWidth;x++) {
    //
    //         const float noiseVal = noise.GetNoise(static_cast<float>(x), static_cast<float>(z)) + 1.0f;
    //
    //         const unsigned int blockHeight = (noiseVal / 2.0f) * Config::chunkMaxBlockHeight;
    //
    //         noiseData[z][x] = blockHeight;
    //
    //     }
    //
    // }
    //
    // for(int chunkPosX = -Config::chunkRadius; chunkPosX < Config::chunkRadius; chunkPosX++) {
    //     for(int chunkPosZ = -Config::chunkRadius; chunkPosZ < Config::chunkRadius; chunkPosZ++) {
    //             m_chunks.insert({glm::ivec2(chunkPosX,chunkPosZ),ChunkColumn(noiseData,chunkPosX,chunkPosZ)});
    //     }
    // }
    //
    //
    // setNeighbours();
    //
    // for(auto &pair:m_chunks) {
    //     ChunkColumn &chunk = pair.second;
    //     chunk.generateMesh();
    // }



}


World::~World() {

}

void World::setNeighbours() {
    for(auto& pair : m_chunks) {
        glm::ivec2 pos = pair.first;
        ChunkColumn& chunkColumn = pair.second;


        ChunkColumn* chunkNx = nullptr;
        ChunkColumn* chunkPx = nullptr;
        ChunkColumn* chunkNz = nullptr;
        ChunkColumn* chunkPz = nullptr;


        auto itNx = m_chunks.find(glm::ivec2(pos.x - 1,  pos.y));
        if (itNx != m_chunks.end()) {
            chunkNx = &(itNx->second);
        }


        auto itPx = m_chunks.find(glm::ivec2(pos.x + 1,  pos.y));
        if (itPx != m_chunks.end()) {
            chunkPx = &(itPx->second);
        }



        auto itNz = m_chunks.find(glm::ivec2(pos.x,  pos.y - 1));
        if (itNz != m_chunks.end()) {
            chunkNz = &(itNz->second);

        }


        auto itPz = m_chunks.find(glm::ivec2(pos.x,pos.y + 1));
        if (itPz != m_chunks.end()) {
            chunkPz = &(itPz->second);
        }

        chunkColumn.setNeighbouringChunks(chunkNx, chunkPx, chunkNz, chunkPz);
    }
}

const ChunkMap &World::getChunks()const {
    return m_chunks;
}

